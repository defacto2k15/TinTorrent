//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_SOCKETWRAPPER_H
#define TINTORRENT_SOCKETWRAPPER_H


#include <unistd.h>
#include "../Common/Typedefs.h"
#include "../Assertions/Assertions.h"

class SocketWrapper {
protected:
	socket_descriptor_t  socket;
	SocketWrapper() : socket(nullptr){
	};

	void closeSocket(){
		if( socket != nullptr){
			int closeRes = close(socket);
			Assertions::check([closeRes](){return closeRes == 0;}, "Closing of socket failed");
			socket = nullptr;
		}
	}

public:
	virtual ~SocketWrapper() {
		closeSocket();
	}
};


#endif //TINTORRENT_SOCKETWRAPPER_H
