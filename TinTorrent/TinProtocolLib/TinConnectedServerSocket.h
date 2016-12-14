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
#include "../Common/Typedefs.h"
#include "../Assertions/Assertions.h"
#include "../Common/InMemoryBuffer.h"
#include "Messages/SegmentResponse.h"
#include "SocketWrapper.h"
#include "TinConnectedSocket.h"
#include "Segment.h"

class TinConnectedServerSocket  : public TinConnectedSocket {
	sockaddr_in clientAddress;
public:
	TinConnectedServerSocket(socket_descriptor_t socket, sockaddr_in clientAddress)
			: TinConnectedSocket(socket), clientAddress(clientAddress){
	}

	Resource listenForResourceRequest(){
		readToBuffer();
		auto request = deserializeFromBuffer<MessageResourceRequest>();
		return request.getResource();
	}

	void sendResourceResponse( MessageResourceResponse::ResourceResponseValue responseValue ){
		MessageResourceResponse response(responseValue);
		serializeToBuffer(response);
		sendBuffer();
	};

	MessageStartSendingRequest listenForStartSendingRequest(){
		readToBuffer();
		return deserializeFromBuffer<MessageStartSendingRequest >();
	}
	void sendSegmentResponse( SegmentInfo segmentInfo, uint8_t * payload){
		SegmentResponse::writeSegmentResponseToBuffer( segmentInfo, payload, buffer);
		sendBuffer();
	}
};


#endif //TINTORRENT_TINCONNECTEDSERVERSOCKET_H
