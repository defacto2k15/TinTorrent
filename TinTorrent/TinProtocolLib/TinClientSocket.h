//
// Created by defacto on 09.12.16.
//

#ifndef TINTORRENT_TINCLIENTSOCKET_H
#define TINTORRENT_TINCLIENTSOCKET_H


#include <netinet/in.h>
#include "../Common/Typedefs.h"
#include "../ModelEntities/TinAddress.h"
#include "../Assertions/Assertions.h"
#include "SocketWrapper.h"
#include "TinConnectedClientSocket.h"

class TinClientSocket : SocketWrapper{
	socket_descriptor_t  socket;
	TinAddress addressToConnect;
public:
	TinClientSocket(socket_descriptor_t socket, const TinAddress &addressToConnect);

	void init(){
		socket = socket(AF_INET, SOCK_STREAM, 0);
		Assertions::check( [socket](){ return socket != -1;}, "Opening socket failed");
	}

	void connect(){
		Assertions::check( [socket](){return socket != 0}, "Socket not initialized");
		sockaddr_in server = addressToConnect.getSockaddr();
		int connectResult = connect(socket, (struct sockaddr *)&server, sizeof(server));
		Assertions::check( [connectResult](){ return socket == 0;}, "Connect failed");
		socket_descriptor_t  tempSocket = socket;
		socket = 0;
		return  TinConnectedClientSocket( tempSocket);
	}
};


#endif //TINTORRENT_TINCLIENTSOCKET_H
