//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_TINCONNECTEDSERVERSOCKET_H
#define TINTORRENT_TINCONNECTEDSERVERSOCKET_H


#include <netinet/in.h>
#include "Messages/MessageResourceResponse.h"
#include "Messages/MessageResourceRequest.h"
#include "Messages/MessageStartSendingRequest.h"
#include "SegmentInfo.h"
#include "Messages/MessageClose.h"
#include "../Common/Typedefs.h"
#include "../Assertions/Assertions.h"
#include "../Common/InMemoryBuffer.h"
#include "Messages/SegmentResponse.h"
#include "SocketWrapper.h"

class TinConnectedServerSocket  : public SocketWrapper{
	sockaddr_in clientAddress;
	InMemoryBuffer buffer;
	// TODO buffer size from configuration
	// todo destruction of socket
public:
	TinConnectedServerSocket(socket_descriptor_t socket, sockaddr_in clientAddress)
			: socket(socket), clientAddress(clientAddress), buffer( new char[5000], 5000){
	}

	MessageResourceRequest acceptConnections(){
		return MessageResourceRequest( buffer );
	}

	void respondToResourceRequest( MessageResourceResponse::ResourceResponseValue responseValue ){
		MessageResourceResponse response(responseValue);
		response.serializeTo(buffer);
		sendBuffer();
	};

	MessageStartSendingRequest listenForStartSendingRequest(){
		readToBuffer();
		return MessageStartSendingRequest(buffer);
	}
	void sendSegmentResponse( SegmentInfo info ){
		SegmentResponse *UNUSED_response
				          = new( buffer.getData)SegmentResponse(info.getSegmentIndex(),
				                                                info.getPayloadLength(), (uint8_t *)info.getPayload());
		//todo is it working???????
		sendBuffer();
	}

	void closeConnection( MessageClose::CloseReason closeReason ){
		MessageClose messageClose(closeReason);
		messageClose.serializeTo(buffer);
		sendBuffer();
	}
private:
	void sendBuffer(){
		ssize_t sendBytes = send(socket, buffer.getData(), buffer.getSize(), 0);
		Assertions::check([sendBytes, &buffer]{return sendBytes == buffer.getSize();}, "Sending failed");
	}

	void readToBuffer(){
		ssize_t readSize = recv( socket, buffer.getData(), buffer.getMaxSize(), 0);
		Assertions::check([readSize](){ return readSize != -1;}, "Recieving message failed");
		buffer.setSize((size_t )readSize);
	}
};


#endif //TINTORRENT_TINCONNECTEDSERVERSOCKET_H
