//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_TINPROTOCOLCOMMUNICATIONSOCKET_H
#define TINTORRENT_TINPROTOCOLCOMMUNICATIONSOCKET_H


#include "Messages/MessageResourceRequest.h"
#include "Messages/MessageEnum.h"
#include "Messages/MessageStartSendingRequest.h"
#include "SegmentInfo.h"
#include "Messages/MessageClose.h"
#include "Messages/MessageResourceResponse.h"
#include "../Assertions/Assertions.h"
#include "../Common/Typedefs.h"
#include "TinConnectedServerSocket.h"
#include <string>

class TinServerSocket : public SocketWrapper{
private:
    uint16_t portNumber;
    // TODO configurationProvider
	// TODO add state of serverSocket Inited, Listening itd
public:
    TinServerSocket(uint16_t portNumber) : portNumber(portNumber) {}

	void initSocket(){
		socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
		Assertions::check( [this](){ return socketDescriptor != -1;}, "Opening socketDescriptor failed");

		sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		serverAddr.sin_port = htons(portNumber);

		int bindResult = bind(socketDescriptor, (struct sockaddr*)&serverAddr, sizeof(serverAddr ) );
		Assertions::check( [bindResult](){ return bindResult == 0 ;}, "Binding socketDescriptor failed");

		// todo configure backlog
		int listenResult = listen( socketDescriptor, 10);
		Assertions::check( [listenResult](){ return listenResult == 0; }, "Listen method failed");
	}

	std::shared_ptr<TinConnectedServerSocket> listenForConnections(){
		struct sockaddr_in client;
		socklen_t socklen = sizeof(struct sockaddr_in);
		socket_descriptor_t associatedDescriptor
				= accept( socketDescriptor, (struct sockaddr *)&client,  &socklen);
		Assertions::check([associatedDescriptor]() { return associatedDescriptor != -1;}, "Funkcja accept nie powiadla sie");
		return std::make_shared<TinConnectedServerSocket>(associatedDescriptor, client);
	}
};



#endif //TINTORRENT_TINPROTOCOLCOMMUNICATIONSOCKET_H
