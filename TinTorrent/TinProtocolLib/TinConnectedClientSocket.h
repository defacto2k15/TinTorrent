//
// Created by defacto on 09.12.16.
//

#ifndef TINTORRENT_TINCONNECTEDCLIENTSOCKET_H
#define TINTORRENT_TINCONNECTEDCLIENTSOCKET_H


#include "TinConnectedSocket.h"
#include "Messages/MessageResourceResponse.h"
#include "Messages/MessageStartSendingRequest.h"
#include "SegmentInfo.h"
#include "Messages/SegmentResponse.h"

class TinConnectedClientSocket : TinConnectedSocket{

public:
	TinConnectedClientSocket(socket_descriptor_t socket);

	void sendResourceRequest(Resource resource ){
		MessageResourceRequest request(resource);
		request.serializeTo(buffer);
		sendBuffer();
	}

	MessageResourceResponse::ResourceResponseValue listenForResourceResponse(){
		readToBuffer();
		MessageResourceResponse response(buffer);
		return response.getValue();
	}

	void sendStartSendingRequest( SegmentInfo segment ){
		MessageStartSendingRequest request(segment);
		request.serializeTo(buffer);
		sendBuffer();
	}

	std::pair<SegmentInfo, uint8_t*> listenForSegmentResponse(){
		readToBuffer();
		SegmentResponse response(buffer);
		return std::make_pair(response.getSegmentInfo(), response.getDataPointer());
	};
};


#endif //TINTORRENT_TINCONNECTEDCLIENTSOCKET_H
