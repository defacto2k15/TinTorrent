//
// Created by defacto on 09.12.16.
//

#include "TinClientSocket.h"

TinClientSocket::TinClientSocket( const TinAddress &addressToConnect) : addressToConnect( addressToConnect) {}

void TinClientSocket::initSocket() {
	socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
//		if( !Constants::interfaceName.empty()){
//			const char *interfaceName =  Constants::interfaceName.c_str();
//			int res = setsockopt(socketDescriptor, SOL_SOCKET, SO_BINDTODEVICE, interfaceName, strlen(interfaceName));
//			Assertions::check(res != -1, "Failed binding socket to ")
//		}
	Assertions::check<SocketCommunicationException>( [this](){ return socketDescriptor != -1;}, "Opening socketDescriptor failed");
}

socket_descriptor_t TinClientSocket::connect() {
	Assertions::check<SocketCommunicationException>( [this](){return socketDescriptor != INVALID;}, Help::Str("Socket not initialized as it is ", socketDescriptor));
	sockaddr_in server = addressToConnect.getSockaddr();

	connectWithTimeout (server);
	//Assertions::check<SocketCommunicationException>( [connectResult](){ return connectResult == 0;}, "Connect failed");
	socket_descriptor_t  tempSocket = socketDescriptor;
	socketDescriptor = 0;
	return  tempSocket;
}

int TinClientSocket::connectWithTimeout(sockaddr_in server) {
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
		tv.tv_sec = Constants::connectTimeout;
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
