//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_TINCONNECTEDSERVERSOCKET_H
#define TINTORRENT_TINCONNECTEDSERVERSOCKET_H


#include <netinet/in.h>
#include <ModelEntities/TinAddress.h>
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
	TinConnectedServerSocket(socket_descriptor_t socket, sockaddr_in clientAddress);

	Resource listenForResourceRequest();

	void sendResourceResponse( MessageResourceResponse::ResourceResponseValue responseValue );;

	MessageStartSendingRequest listenForStartSendingRequest();

	void sendSegmentResponse( SegmentInfo segmentInfo, uint8_t * payload);

	TinAddress getClientAddress();

};


#endif //TINTORRENT_TINCONNECTEDSERVERSOCKET_H
