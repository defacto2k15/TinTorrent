//
// Created by defacto on 31.12.16.
//

#ifndef TINTORRENT_TINBROADCASTSOCKET_H
#define TINTORRENT_TINBROADCASTSOCKET_H

#include <netinet/in.h>
#include <ModelEntities/Resource.h>
#include <TinProtocolLib/Messages/BroadcastMessage.h>
#include <Common/InMemoryBuffer.h>
#include <ModelEntities/TinAddress.h>
#include "SocketWrapper.h"
#include "SocketCommunicationException.h"

class TinBroadcastSocket : public SocketWrapper {
	TinAddress destinationAddress;
	InMemoryBuffer messagesBuffer;
public:
	TinBroadcastSocket( TinAddress destinationAddress );

	void initSocket();

	void sendAnnounceMessage( std::vector<Resource> &resources );

	void sendRevertMessage( std::vector<Resource> &resources  );

private:
	void sendMessage(std::vector<Resource> &resources, BroadcastMessage::BroadcastType type);

	void writeMessageToBuffer( BroadcastMessage &message);

	void broadcastBuffer();

};


#endif //TINTORRENT_TINBROADCASTSOCKET_H
