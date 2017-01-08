//
// Created by defacto on 01.01.17.
//

#include "TinConnectedServerThread.h"
#include "Kernel/Kernel.h"

TinConnectedServerThread::TinConnectedServerThread(Kernel &kernel,
                                                   std::shared_ptr<TinConnectedServerSocket> connectedServerSocket,
                                                   int threadId)
		: ActionQueue(this), log(Help::Str("ConnectedServerThread ", threadId)), 
		  kernel(kernel), connectedServerSocket(connectedServerSocket), threadId(threadId){
}

void TinConnectedServerThread::listenForResourceRequest() {
	handleException( [this](){
		log.debug(" starting listening for resource request");
		MessageResourceRequest request = connectedServerSocket->listenForResourceRequest();
		requestedResource = request.getResource();
		log.debug(" got request: ",request.toJson().dump());
		kernel.add([ this](Kernel &k){ k.gotResourceRequest(threadId, requestedResource);});
		isConnectionOpen = true;
	});
}

void TinConnectedServerThread::sendResourceResponse(MessageResourceResponse::ResourceResponseValue responseValue) {
	handleException( [this, responseValue](){
		log.debug(" sending resource response ",responseValue.getValue());
		connectedServerSocket->sendResourceResponse(responseValue);
		log.debug(" listening for segment request");
		MessageStartSendingRequest startSendingRequest = connectedServerSocket->listenForStartSendingRequest();
		log.debug(" got segment request: ",startSendingRequest.toJson().dump());
		kernel.add([startSendingRequest, this](Kernel &k){ k.gotStartSendingRequest(threadId, startSendingRequest);});
	});
}

void TinConnectedServerThread::sendSegments(SegmentsSet segmentsSet) {
	handleException( [this, &segmentsSet](){
		log.debug(" sending segments of range",segmentsSet.getRange());

		for( unsigned int i = segmentsSet.getRange().getMin(); i < segmentsSet.getRange().getMax(); i++){
			log.debug(" sending segment of index ",i);
			auto segmentInfo = segmentsSet.getSegment(i).getSegmentInfo();
			auto payload = segmentsSet.getSegment(i).getPayload().getData();
			connectedServerSocket->sendSegmentResponse(segmentInfo, payload);

			log.debug(" segment sent, now listening for start sending request");
			auto startSendingRequest = connectedServerSocket->listenForStartSendingRequest();
			log.debug(" got segment request: ",startSendingRequest.toJson().dump());
			if( startSendingRequest.getMessageType() == MessageStartSendingRequest::PreviousStatus::OK ){
				if( i == segmentsSet.getRange().getMax() - 1 ){ // last one
					log.debug(" Asking kernel for next segments");
					kernel.add([startSendingRequest, this](Kernel &k){ k.gotStartSendingRequest(threadId, startSendingRequest);});
				}
			}else {
				kernel.add([startSendingRequest, this](Kernel &k){ k.gotStartSendingRequest(threadId, startSendingRequest);});
			}
		}
	});

}

void TinConnectedServerThread::listenForCloseMessage() {
	log.debug(" waiting for close message ");
	handleException( [this]() {
		connectedServerSocket->listenForResourceRequest(); //todo create proper lsiten for close message
	});
}

void TinConnectedServerThread::sendCloseMessage(MessageClose::CloseReason reason) {
	log.debug(" closing connection, reason ",reason.getValue());
	handleException( [this, reason]() {
		connectedServerSocket->closeConnection(reason);
	});
	isConnectionOpen = false;
}

void TinConnectedServerThread::genericClose() {
	try{
		connectedServerSocket->closeConnection(MessageClose::CloseReason::OK);
		isConnectionOpen = false;
	} catch (...){
		// swallowing exception
	}
}

int TinConnectedServerThread::getThreadId() {
	return threadId;
}

Resource TinConnectedServerThread::getRequestedResource() {
	return requestedResource;
}

void TinConnectedServerThread::handleException(std::function<void()> func) {

	try{
		try{
			func();
		} catch( MessageCloseException &closeException){
			log.debug(" recieved close message ", closeException.getMessageClose().toJson().dump());
			kernel.add([this](Kernel &k){ k.serverCommunicationClosed(threadId);});
		} catch( SocketCommunicationException &ex ) {
			isConnectionOpen = false;
			std::cout<<Help::Str("ConnectedServerThread got SCE exception ", ex.what());
			if (!threadShouldRun) {
				std::cout<<"ConnectedServerThread. ThreadShouldRun is falce, so exiting ";
				return;
			} else {
				throw ex;
			}
		}
	} catch( std::exception &e){
		log.debug(" listening resource request failed: ", e.what());
		kernel.add([this](Kernel &k){ k.serverCommunicationFailure(threadId);});
		connectedServerSocket->closeConnection( MessageClose::CloseReason::JSON_DESERIALIZATION);
		isConnectionOpen = false;
	}
}

TinAddress TinConnectedServerThread::getConnectedAddress() {
	return connectedServerSocket->getClientAddress();
}

void TinConnectedServerThread::internalKillYourself() {
	log.debug(" Got message internalKillYourself");
	if( connectedServerSocket ){
		connectedServerSocket->shutdownSocket();
	}
}

bool TinConnectedServerThread::getIsConnectionOpen() {
	return isConnectionOpen;
}
