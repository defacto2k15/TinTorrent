//
// Created by defacto on 09.12.16.
//

#ifndef TINTORRENT_TINCONNECTEDSOCKET_H
#define TINTORRENT_TINCONNECTEDSOCKET_H


#include <sys/socket.h>
#include "SocketWrapper.h"
#include "Messages/MessageResourceRequest.h"
#include "Messages/MessageClose.h"

class TinConnectedSocket : SocketWrapper {
	socket_descriptor_t  socket;

protected:
	InMemoryBuffer buffer;

	void readToBuffer(){
		ssize_t readSize = recv( socket, buffer.getData(), buffer.getMaxSize(), 0);
		Assertions::check([readSize](){ return readSize != -1;}, "Recieving message failed");
		buffer.setSize((size_t )readSize);
	}

	void sendBuffer(){
		ssize_t sendBytes = send(socket, buffer.getData(), buffer.getSize(), 0);
		Assertions::check([sendBytes, &buffer]{return sendBytes == buffer.getSize();}, "Sending failed");
	}

public:
	// TODO buffer size from configuration
	TinConnectedSocket( socket_descriptor_t socket ) : socket(socket ), buffer( new char[5000], 5000){};

	void closeConnection( MessageClose::CloseReason closeReason ){
		MessageClose messageClose(closeReason);
		messageClose.serializeTo(buffer);
		sendBuffer();
	}
};


#endif //TINTORRENT_TINCONNECTEDSOCKET_H
