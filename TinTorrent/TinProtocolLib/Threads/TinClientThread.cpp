//
// Created by defacto on 01.01.17.
//

#include "TinClientThread.h"
#include <Kernel/Kernel.h>

TinClientThread::TinClientThread(TinAddress &addressToConnect, Kernel &kernel) : ActionQueue(this, Help::Str("TinClientThread of ",addressToConnect)),
                                                                                 log(Help::Str("TinClientThread of ",addressToConnect)), 
                                                 clientSocket(addressToConnect), addressToConnect(addressToConnect), kernel(kernel){
}

void TinClientThread::startDownloadingProcess(Resource resource, SegmentRange segmentsToDownload) {
	log.debug(" connect() ");
	try {
		clientSocket.initSocket();
		log.debug(" done init");
		connectedSocket = std::make_unique<TinConnectedClientSocket>(clientSocket.connect());
		log.debug(" done connect");
		isConnectionOpen = true;
		requestedResource = resource;
		requestedSegments = segmentsToDownload;
		sendResourceRequest( );
	} catch (std::exception &e ){
		log.debug(" connecting failure: ",e.what());
		kernel.add([this](Kernel &k){ k.connectingToClientFailed(addressToConnect);});
		isConnectionOpen = false;
		return;
	}
}

void TinClientThread::sendResourceRequest() {
	handleException([this](){
		log.debug(" sending resource request: ", requestedResource.toJson().dump());
		connectedSocket->sendResourceRequest(requestedResource);
		log.debug(" listening for response");
		MessageResourceResponse::ResourceResponseValue responseValue = connectedSocket->listenForResourceResponse();
		log.debug(" got response ",responseValue.getValue());
		if( responseValue.getValue() != MessageResourceResponse::ResourceResponseValue::OK.getValue()){
			kernel.add([responseValue, this](Kernel &k){
				k.gotBadResourceResponse(addressToConnect, responseValue);
			});
			return;
		}
	});
	recieveSegments(requestedSegments, MessageStartSendingRequest::PreviousStatus::FIRST);
}

void TinClientThread::recieveSegments(SegmentRange segmentRange, MessageStartSendingRequest::PreviousStatus prevStatus) {
	requestedSegments = segmentRange;
	handleException([segmentRange, this, prevStatus](){
		log.debug(" start recieving segments:  ", segmentRange );

		std::vector<Segment> outSegmentsVec;
		unsigned badRecievedSegmentsCount = 0;
		for(unsigned segmentIndex = segmentRange.getMin(); segmentIndex < segmentRange.getMax(); segmentIndex++){
			SegmentInfo requestedInfo((uint16_t)segmentIndex, SegmentUtils::SegmentSize(requestedResource.getResourceSize(), (uint16_t )segmentIndex));
			connectedSocket->sendStartSendingRequest(requestedInfo, prevStatus);
			log.debug(" sent segment request: ", segmentIndex," waiting for response" );
			SegmentResponse *segmentResponse = connectedSocket->listenForSegmentResponse();
			log.debug(" got response. Info: ",
			                       segmentResponse->getSegmentInfo().toJson().dump() );

			auto recievedSegmentInfo = segmentResponse->getSegmentInfo();
			if( recievedSegmentInfo != requestedInfo ){
				badRecievedSegmentsCount++;
				log.debug(" response segment info was not equal to expected: ",
				                       requestedInfo.toJson().dump(), " It is done for ",badRecievedSegmentsCount," time" );
				if( badRecievedSegmentsCount > Constants::maxBadRecievedSegmentsBeforeDisconnect ){
					kernel.add([this](Kernel &k){ k.clientCommunicationFailure(addressToConnect);});
					log.debug(" Closing connection");
					connectedSocket->closeConnection( MessageClose::CloseReason::WRONG_SEGMENT);
					break;
				}
				segmentIndex--;
			} else {
				badRecievedSegmentsCount = 0;
				std::shared_ptr<Buffer> segmentPayload = std::make_shared<InMemoryBuffer>(Constants::segmentSize);
				segmentPayload->setData( segmentResponse->payload, segmentResponse->payloadLength);
				outSegmentsVec.push_back(Segment(segmentPayload, requestedInfo));
			}
		}
		kernel.add( [outSegmentsVec, segmentRange, this]( Kernel &k ){
			k.recievedSegments(addressToConnect, requestedResource, SegmentsSet(segmentRange, outSegmentsVec));});
	});
}

void TinClientThread::closeConnection(MessageClose::CloseReason closeReason) {
	log.debug(" closing connection with reason ", closeReason.getValue() );
	handleException([closeReason, this](){
		if( connectedSocket) {
			connectedSocket->closeConnection(closeReason);
		}
	});
	isConnectionOpen = false;
}

Resource TinClientThread::getRequestedResource() const {
	return requestedResource;
}

SegmentRange TinClientThread::getRequestedSegments() const {
	return requestedSegments;
}

void TinClientThread::genericCloseConnection() {
	try{
		if( connectedSocket) {
			connectedSocket->closeConnection(MessageClose::CloseReason::OK);
		}
		isConnectionOpen = false;
	} catch (...){
		// swallowing exception
	}
}

void TinClientThread::handleException(std::function<void()> func) { //ugly code duplication
	try{
		func();
	} catch( MessageCloseException &closeException){
		log.debug(" recieved close message ", closeException.getMessageClose().toJson().dump());
		MessageClose::CloseReason reason = closeException.getMessageClose().getReason();
		kernel.add([this, reason](Kernel &k){ k.clientCommunicationClosed(addressToConnect, reason);});
	} catch( std::exception &e){
		log.debug(" listening resource request failed: ", e.what());
		kernel.add([this](Kernel &k){ k.clientCommunicationFailure(addressToConnect);});
		if( connectedSocket) {
			connectedSocket->closeConnection(MessageClose::CloseReason::JSON_DESERIALIZATION);
		}
		isConnectionOpen = false;
	}
}

