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

class TinConnectedClientSocket : public TinConnectedSocket{

public:
	TinConnectedClientSocket(socket_descriptor_t socket);

	void sendResourceRequest(Resource resource ){
		MessageResourceRequest request(resource);
		serializeToBuffer(request);
		sendBuffer();
	}

	MessageResourceResponse::ResourceResponseValue listenForResourceResponse(){
		readToBuffer();
		auto response = deserializeFromBuffer<MessageResourceResponse >();
		return response.getValue();
	}

	void sendStartSendingRequest( SegmentInfo segment, uint8_t *payload){
		MessageStartSendingRequest request(segment);
		serializeToBuffer(request);
		sendBuffer();
	}

	SegmentInfo listenForSegmentResponse(){
		readToBuffer();
		auto response = deserializeFromBuffer<SegmentResponse >();
		return response.getSegmentInfo();
	};
};


#endif //TINTORRENT_TINCONNECTEDCLIENTSOCKET_H
