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

	std::pair<SegmentInfo, MessageStartSendingRequest::Type> listenForStartSendingRequest(){
		readToBuffer();
		auto request = deserializeFromBuffer<MessageStartSendingRequest >();
		return request.getInfoPair();
	}
	void sendSegmentResponse( SegmentInfo info ){
		SegmentResponse response(info);
		serializeToBuffer(response);
		sendBuffer();
	}

};


#endif //TINTORRENT_TINCONNECTEDSERVERSOCKET_H
