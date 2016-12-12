//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_SOCKETWRAPPER_H
#define TINTORRENT_SOCKETWRAPPER_H


#include <unistd.h>
#include "../Common/Typedefs.h"
#include "../Assertions/Assertions.h"

class SocketWrapper {
	const socket_descriptor_t INVALID = -1;
protected:
	socket_descriptor_t  socketDescriptor;
	SocketWrapper() : socketDescriptor(INVALID){
	};
public:
	void closeSocket(){
		if( socketDescriptor != INVALID){
			int closeRes = close(socketDescriptor);
			Assertions::check([closeRes](){return closeRes == 0;}, "Closing of socketDescriptor failed");
			socketDescriptor = INVALID;
		}
	}

	virtual ~SocketWrapper() {
		closeSocket();
	}
};


#endif //TINTORRENT_SOCKETWRAPPER_H
