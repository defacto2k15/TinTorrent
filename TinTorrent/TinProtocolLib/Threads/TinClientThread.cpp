//
// Created by defacto on 01.01.17.
//

#include "TinClientThread.h"
#include <Kernel/Kernel.h>

TinClientThread::TinClientThread(TinAddress &addressToConnect, Kernel &kernel) : ActionQueue(this), clientSocket(addressToConnect),
                                                                                 addressToConnect(addressToConnect), kernel(kernel){
}

void TinClientThread::startDownloadingProcess(Resource resource, SegmentRange segmentsToDownload) {
	std::cout << Help::Str("TinClientThread of ", addressToConnect," connect() ") << std::endl;
	try {
		clientSocket.initSocket();
		std::cout << Help::Str("TinClientThread of ", addressToConnect," done init") << std::endl;
		connectedSocket = std::make_unique<TinConnectedClientSocket>(clientSocket.connect());
		std::cout << Help::Str("TinClientThread of ", addressToConnect," done connect") << std::endl;
		isConnectionOpen = true;
		requestedResource = resource;
		requestedSegments = segmentsToDownload;
		sendResourceRequest( );
	} catch (std::exception &e ){
		std::cout << Help::Str("TinClientThread of ", addressToConnect," connecting failure: ",e.what()) << std::endl;
		kernel.add([this](Kernel &k){ k.connectingToClientFailed(addressToConnect);});
		isConnectionOpen = false;
		return;
	}
}

void TinClientThread::sendResourceRequest() {
	handleException([this](){
		std::cout << Help::Str("TinClientThread of ", addressToConnect," sending resource request: ", requestedResource.toJson().dump()) << std::endl;
		connectedSocket->sendResourceRequest(requestedResource);
		std::cout << Help::Str("TinClientThread of ", addressToConnect," listening for response") << std::endl;
		MessageResourceResponse::ResourceResponseValue responseValue = connectedSocket->listenForResourceResponse();
		std::cout << Help::Str("TinClientThread of ", addressToConnect," got response ",responseValue.getValue()) << std::endl;
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
		std::cout << Help::Str("TinClientThread of ", addressToConnect," start recieving segments:  ", segmentRange ) << std::endl;

		std::vector<Segment> outSegmentsVec;
		unsigned badRecievedSegmentsCount = 0;
		for(unsigned segmentIndex = segmentRange.getMin(); segmentIndex < segmentRange.getMax(); segmentIndex++){
			SegmentInfo requestedInfo((uint16_t)segmentIndex, SegmentUtils::SegmentSize(requestedResource.getResourceSize(), (uint16_t )segmentIndex));
			connectedSocket->sendStartSendingRequest(requestedInfo, prevStatus);
			std::cout << Help::Str("TinClientThread of ", addressToConnect," sent segment: ", segmentRange," waiting for response" ) << std::endl;
			SegmentResponse *segmentResponse = connectedSocket->listenForSegmentResponse();
			std::cout << Help::Str("TinClientThread of ", addressToConnect," got response. Info: ",
			                       segmentResponse->getSegmentInfo().toJson().dump() ) << std::endl;

			auto recievedSegmentInfo = segmentResponse->getSegmentInfo();
			if( recievedSegmentInfo != requestedInfo ){
				badRecievedSegmentsCount++;
				std::cout << Help::Str("TinClientThread of ", addressToConnect," response segment info was not equal to expected: ",
				                       requestedInfo.toJson().dump(), " It is done for ",badRecievedSegmentsCount," time" ) << std::endl;
				if( badRecievedSegmentsCount > Constants::maxBadRecievedSegmentsBeforeDisconnect ){
					kernel.add([this](Kernel &k){ k.clientCommunicationFailure(addressToConnect);});
					std::cout << Help::Str("TinClientThread of ", addressToConnect," Closing connection") << std::endl;
					connectedSocket->closeConnection( MessageClose::CloseReason::WRONG_SEGMENT);
					break;
				}
				segmentIndex--;
			} else {
				badRecievedSegmentsCount = 0;
				std::shared_ptr<Buffer> segmentPayload = std::make_shared<InMemoryBuffer>(Constants::segmentSize);
				segmentPayload->setData( segmentResponse->payload, Constants::segmentSize);
				outSegmentsVec.push_back(Segment(segmentPayload, requestedInfo));
			}
		}
		kernel.add( [outSegmentsVec, segmentRange, this]( Kernel &k ){
			k.recievedSegments(addressToConnect, requestedResource, SegmentsSet(segmentRange, outSegmentsVec));});
	});
}

void TinClientThread::closeConnection(MessageClose::CloseReason closeReason) {
	std::cout << Help::Str("TinClientThread ",addressToConnect," closing connection with reason ", closeReason.getValue() ) << std::endl;
	handleException([closeReason, this](){
		connectedSocket->closeConnection(closeReason);
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
		connectedSocket->closeConnection(MessageClose::CloseReason::OK);
		isConnectionOpen = false;
	} catch (...){
		// swallowing exception
	}
}

void TinClientThread::handleException(std::function<void()> func) { //ugly code duplication
	try{
		func();
	} catch( MessageCloseException &closeException){
		std::cout << Help::Str("TinClientThread ",addressToConnect," recieved close message ", closeException.getMessageClose().toJson().dump()) << std::endl;
		MessageClose::CloseReason reason = closeException.getMessageClose().getReason();
		kernel.add([this, reason](Kernel &k){ k.clientCommunicationClosed(addressToConnect, reason);});
	} catch( std::exception &e){
		std::cout << Help::Str("TinClientThread ",addressToConnect," listening resource request failed: ", e.what()) << std::endl;
		kernel.add([this](Kernel &k){ k.clientCommunicationFailure(addressToConnect);});
		connectedSocket->closeConnection( MessageClose::CloseReason::JSON_DESERIALIZATION);
		isConnectionOpen = false;
	}
}

bool TinClientThread::hasOpenedConnection () {
	return  ( connectedSocket && isConnectionOpen);
}
