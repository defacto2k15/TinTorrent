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

	void initSocket(){
		socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
		Assertions::check<SocketCommunicationException>( [this](){ return socketDescriptor != -1;}, "Opening socketDescriptor failed");
	}

	socket_descriptor_t connect(){
		Assertions::check<SocketCommunicationException>( [this](){return socketDescriptor != INVALID;}, Help::Str("Socket not initialized as it is ", socketDescriptor));
		sockaddr_in server = addressToConnect.getSockaddr();

		int connectResult = ::connect(socketDescriptor, (struct sockaddr *)&server, sizeof(server));
		Assertions::check( [connectResult](){ return connectResult == 0;}, "Connect failed");
		//Assertions::check<SocketCommunicationException>( [connectResult](){ return connectResult == 0;}, "Connect failed");
		socket_descriptor_t  tempSocket = socketDescriptor;
		socketDescriptor = 0;
		return  tempSocket;
	}

	int connectWithTimeout( sockaddr_in server ){
		int res;
		long arg;
		fd_set myset;
		struct timeval tv;
		int valopt;
		socklen_t lon;

		// Set non-blocking
		arg = fcntl(socketDescriptor, F_GETFL, NULL);
		Assertions::check<SocketCommunicationException>(arg >= 0 ,"Error fcntl(..., F_GETFL)");

		arg |= O_NONBLOCK;
		auto arg2  = fcntl(socketDescriptor, F_SETFL, arg);
		Assertions::check<SocketCommunicationException>(arg >= 0 ,"Error fcntl(..., F_SETFL)");

		// Trying to connect with timeout
		res = ::connect(socketDescriptor, (struct sockaddr *)&server, sizeof(server));
		if (res < 0) {
			Assertions::check<SocketCommunicationException>( errno == EINPROGRESS, "Error connecting as errno is not set to in progress");
			tv.tv_sec = 10; //todo configure
			tv.tv_usec = 0;
			FD_ZERO(&myset);
			FD_SET(socketDescriptor, &myset);
			res = select(socketDescriptor+1, NULL, &myset, NULL, &tv);
			Assertions::check<SocketCommunicationException>(res > 0 || errno == EINTR, "Error connecting 2");
			Assertions::check<SocketCommunicationException>(res != 0, "Timeout in connection");

			// Socket selected for write
			lon = sizeof(int);
			arg = getsockopt(socketDescriptor, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
			// Check the value returned...
			Assertions::check<SocketCommunicationException>(res >= 0 , "Error in getsockopt");
			Assertions::check<SocketCommunicationException>(!valopt,
			                                                Help::Str("Error in delayed connection: valopt: ",valopt," str ", strerror(valopt)));
		}
		// Set to blocking mode again...
		arg = fcntl(socketDescriptor, F_GETFL, NULL);
		Assertions::check<SocketCommunicationException>(arg2 >= 0 ,"Error fcntl(..., F_GETFL)");
		arg &= (~O_NONBLOCK);
		arg2 = fcntl(socketDescriptor, F_SETFL, arg) ;
		Assertions::check<SocketCommunicationException>(arg2 >= 0 ,"Error fcntl(..., F_SETFL");
		return res;
	}
};


#endif //TINTORRENT_TINCLIENTSOCKET_H
