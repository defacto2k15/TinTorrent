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
		MessageResourceRequest request( buffer );
		return request.getResource();
	}

	void sendResourceResponse( MessageResourceResponse::ResourceResponseValue responseValue ){
		MessageResourceResponse response(responseValue);
		response.serializeTo(buffer);
		sendBuffer();
	};

	std::pair<SegmentInfo, MessageStartSendingRequest::Type> listenForStartSendingRequest(){
		readToBuffer();
		MessageStartSendingRequest request(buffer);
		return request.getInfoPair();
	}
	void sendSegmentResponse( SegmentInfo info ){
		SegmentResponse *UNUSED_response
				          = new( buffer.getData)SegmentResponse(info.getSegmentIndex(),
				                                                info.getPayloadLength(), (uint8_t *)info.getPayload());
		//todo is it working???????
		sendBuffer();
	}

};


#endif //TINTORRENT_TINCONNECTEDSERVERSOCKET_H
