//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_TINCONNECTEDSERVERTHREAD_H
#define TINTORRENT_TINCONNECTEDSERVERTHREAD_H


#include <Kernel/Kernel.h>
#include <TinProtocolLib/TinConnectedServerSocket.h>

class TinConnectedServerThread : ActionQueue<TinConnectedServerThread> {
	Kernel &kernel;
	std::shared_ptr<TinConnectedServerSocket> connectedServerSocket;
	int threadId;
	Resource requestedResource;
public:
	TinConnectedServerThread( Kernel &kernel, std::shared_ptr<TinConnectedServerSocket> connectedServerSocket, int threadId)
			: ActionQueue(this), kernel(kernel), connectedServerSocket(connectedServerSocket), threadId(threadId){
	}

	void listenForResourceRequest(){
		handleException( [this](){
			std::cout << Help::Str("ConnectedServerThread ",threadId," starting listening for resource request") << std::endl;
			MessageResourceRequest request = connectedServerSocket->listenForResourceRequest();
			requestedResource = request.getResource();
			std::cout << Help::Str("ConnectedServerThread ",threadId," got request: ",request.toJson().dump()) << std::endl;
			kernel.add([request](Kernel &k){ k.gotResourceRequest(threadId, requestedResource);});
		});
	}

	void sendResourceResponse(MessageResourceResponse::ResourceResponseValue responseValue){
		handleException( [this](){
			std::cout << Help::Str("ConnectedServerThread ",threadId," sending resource response ",responseValue.getValue()) << std::endl;
			connectedServerSocket->sendResourceResponse(responseValue);
			std::cout << Help::Str("ConnectedServerThread ",threadId," listening for segment request") << std::endl;
			MessageStartSendingRequest startSendingRequest = connectedServerSocket->listenForStartSendingRequest();
			std::cout << Help::Str("ConnectedServerThread ",threadId," got segment request: ",startSendingRequest.toJson().dump()) << std::endl;
			kernel.add([startSendingRequest](Kernel &k){ k.gotStartSendingRequest(threadId, startSendingRequest);});
		});
	}

	void sendSegments( SegmentsSet &segmentsSet ){
		handleException( [this, &segmentsSet](){
			std::cout << Help::Str("ConnectedServerThread ",threadId," sending segments of range",segmentsSet.getRange()) << std::endl;

			for( unsigned int i = segmentsSet.getRange().getMin(); i < segmentsSet.getRange().getMax(); i++){
				std::cout << Help::Str("ConnectedServerThread ",threadId," sending segment of index ",i) << std::endl;
				auto segmentInfo = segmentsSet.getSegment(i).getSegmentInfo();
				auto payload = segmentsSet.getSegment(i).getPayload().getData();
				connectedServerSocket->sendSegmentResponse(segmentInfo, payload);

				std::cout << Help::Str("ConnectedServerThread ",threadId," segment sent, now listening for start sending request") << std::endl;
				auto startSendingRequest = connectedServerSocket->listenForStartSendingRequest();
				std::cout << Help::Str("ConnectedServerThread ",threadId," got segment request: ",startSendingRequest.toJson().dump()) << std::endl;
				if( startSendingRequest.getMessageType() == MessageStartSendingRequest::PreviousStatus::OK ){
					if( i == segmentsSet.getRange().getMax() - 1 ){ // last one
						std::cout << Help::Str("ConnectedServerThread ",threadId," Asking kernel for next segments") << std::endl;
						kernel.add([startSendingRequest](Kernel &k){ k.gotStartSendingRequest(threadId, startSendingRequest);});
					}
				}else {
					kernel.add([startSendingRequest](Kernel &k){ k.gotStartSendingRequest(threadId, startSendingRequest);});
				}
			}
		});

	}

	void listenForCloseMessage(){
		std::cout << Help::Str("ConnectedServerThread ",threadId," waiting for close message ") << std::endl;
		handleException( [this]() {
			connectedServerSocket->listenForResourceRequest(); //todo create proper lsiten for close message
		});
	}

	void sendCloseMessage( MessageClose::CloseReason reason ){
		std::cout << Help::Str("ConnectedServerThread ",threadId," closing connection, reason ",reason.getValue()) << std::endl;
		handleException( [this]() {
			connectedServerSocket->closeConnection(reason);
		});
	}

	int getThreadId(){
		return threadId;
	}

	Resource getRequestedResource(){
		return requestedResource;
	}
private:
	void handleException( std::function<void()> func ){
		try{
			func();
		} catch( MessageCloseException &closeException){
			std::cout << Help::Str("ConnectedServerThread ",threadId," recieved close message ", closeException.getMessageClose().toJson().dump()) << std::endl;
			kernel.add([threadId](Kernel &k){ k.serverCommunicationClosed(threadId);});
		} catch( std::exception &e){
			std::cout << Help::Str("ConnectedServerThread ",threadId," listening resource request failed: ", e.what()) << std::endl;
			kernel.add([threadId](Kernel &k){ k.serverCommunicationFailure(threadId);});
			connectedServerSocket->closeConnection( MessageClose::CloseReason::JSON_DESERIALIZATION);
		}
	}
};


#endif //TINTORRENT_TINCONNECTEDSERVERTHREAD_H
