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

	void sendResourceRequest(const Resource &resource );

	MessageResourceResponse::ResourceResponseValue listenForResourceResponse();

	void sendStartSendingRequest( SegmentInfo segmentInfo, MessageStartSendingRequest::PreviousStatus prevStatus);

	SegmentResponse *listenForSegmentResponse();;
};


#endif //TINTORRENT_TINCONNECTEDCLIENTSOCKET_H
