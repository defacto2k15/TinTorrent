//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_SOCKETWRAPPER_H
#define TINTORRENT_SOCKETWRAPPER_H


#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include "../Common/Typedefs.h"
#include "../Assertions/Assertions.h"

class SocketWrapper {
protected:
	const socket_descriptor_t INVALID = -1;
	socket_descriptor_t  socketDescriptor;
	SocketWrapper();;
public:
	void closeSocket();

	virtual ~SocketWrapper();

	void shutdownSocket();
};


#endif //TINTORRENT_SOCKETWRAPPER_H
