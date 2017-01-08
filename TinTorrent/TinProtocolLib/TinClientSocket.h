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
#include "SocketCommunicationException.h"

class TinClientSocket : public SocketWrapper{
	TinAddress addressToConnect;
public:
	TinClientSocket( const TinAddress &addressToConnect);

	void initSocket(){
		socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
		Assertions::check<SocketCommunicationException>( [this](){ return socketDescriptor != -1;}, "Opening socketDescriptor failed");
	}

	TinConnectedClientSocket connect(){
		Assertions::check<SocketCommunicationException>( [this](){return socketDescriptor != 0;}, "Socket not initialized");
		sockaddr_in server = addressToConnect.getSockaddr();
		int connectResult = ::connect(socketDescriptor, (struct sockaddr *)&server, sizeof(server));
		Assertions::check<SocketCommunicationException>( [connectResult](){ return connectResult == 0;}, "Connect failed");
		socket_descriptor_t  tempSocket = socketDescriptor;
		socketDescriptor = 0;
		return  TinConnectedClientSocket( tempSocket);
	}
};


#endif //TINTORRENT_TINCLIENTSOCKET_H
