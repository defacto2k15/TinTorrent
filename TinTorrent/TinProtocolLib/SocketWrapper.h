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
	SocketWrapper() : socketDescriptor(INVALID){
	};
public:
	void closeSocket(){
		if( socketDescriptor != INVALID && socketDescriptor > 3 ){
			int closeRes = close(socketDescriptor);
			Assertions::check([closeRes](){return closeRes == 0;}, "Closing of socketDescriptor failed");
			socketDescriptor = INVALID;
		}
	}

	virtual ~SocketWrapper() {
		closeSocket();
	}

	void shutdownSocket(){
		if( socketDescriptor != INVALID ){
			auto res = shutdown(socketDescriptor, SHUT_RD);
			if( res == -1 ){
				std::cerr << "Warning: shutdown of socket failed " << std::endl;
			}
		}

	}
};


#endif //TINTORRENT_SOCKETWRAPPER_H
