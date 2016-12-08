//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_TINPROTOCOLCOMMUNICATIONSOCKET_H
#define TINTORRENT_TINPROTOCOLCOMMUNICATIONSOCKET_H


#include <netinet/in.h>
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

// todo change file name
class TinServerSocket : public SocketWrapper{
private:
    uint16_t portNumber;
    socket_descriptor_t socket;
//    const ConfigurationProvider configurationProvider;
    // TODO configurationProvider
    // TODO zwalnianie gniazda przy wyjatkach... (::check)
	// TODO add state of serverSocket Inited, Listening itd
public:
    TinServerSocket(int portNumber) : portNumber(portNumber) {}

	void initSocket(){
		socket = socket(AF_INET, SOCK_STREAM, 0);
		Assertions::check( [socket](){ return socket != -1;}, "Opening socket failed");

		sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		serverAddr.sin_port = htons(portNumber);

		int bindResult = bind(socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr ) );
		Assertions::check( [bindResult](){ return bindResult == 0 ;}, "Bindoing socket failed");

		// todo configure backlog
		int listenResult = listen( socket, 10);
		Assertions::check( [listenResult](){ return listenResult == 0; }, "Listen method failed");
	}

	TinConnectedServerSocket listenForConnections(){
		struct sockaddr_in client;
		socklen_t socklen = sizeof(struct sockaddr_in);
		socket_descriptor_t associatedDescriptor
				= accept( socket, (struct sockaddr *)&client,  &socklen);
		Assertions::check([associatedDescriptor]() { return associatedDescriptor != -1}, "Funkcja accept nie powiadla sie");
		return TinConnectedServerSocket(associatedDescriptor, client);
	}
};



#endif //TINTORRENT_TINPROTOCOLCOMMUNICATIONSOCKET_H
