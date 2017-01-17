//
// Created by defacto on 09.12.16.
//

#ifndef TINTORRENT_TINCLIENTSOCKET_H
#define TINTORRENT_TINCLIENTSOCKET_H


#include <netinet/in.h>
#include <fcntl.h>
#include "../Common/Typedefs.h"
#include "../ModelEntities/TinAddress.h"
#include "../Assertions/Assertions.h"
#include "SocketWrapper.h"
#include "TinConnectedClientSocket.h"
#include "SocketCommunicationException.h"

class TinClientSocket : public SocketWrapper{
	TinAddress addressToConnect;
public:
	TinClientSocket( const TinAddress &addressToConnect);

	void initSocket();

	socket_descriptor_t connect();

	int connectWithTimeout( sockaddr_in server );
};


#endif //TINTORRENT_TINCLIENTSOCKET_H
