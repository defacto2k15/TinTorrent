//
// Created by defacto on 31.12.16.
//

#ifndef TINTORRENT_TINLISTENINGUDPSOCKET_H
#define TINTORRENT_TINLISTENINGUDPSOCKET_H


#include <netinet/in.h>
#include <Common/InMemoryBuffer.h>
#include <Common/Constants.h>
#include <TinProtocolLib/Messages/BroadcastMessage.h>
#include <ModelEntities/TinAddress.h>
#include "SocketWrapper.h"
#include "SocketCommunicationException.h"

class TinListeningUdpSocket : public SocketWrapper{
	InMemoryBuffer messagesBuffer;
public:
	TinListeningUdpSocket();

	void initSocket();

	std::pair<BroadcastMessage, TinAddress> listenForMessages();
private:
	sockaddr_in readToBuffer();


};


#endif //TINTORRENT_TINLISTENINGUDPSOCKET_H
