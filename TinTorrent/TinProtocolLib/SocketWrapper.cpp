//
// Created by defacto on 08.12.16.
//

#include "SocketWrapper.h"

SocketWrapper::SocketWrapper() : socketDescriptor(INVALID){
}

void SocketWrapper::closeSocket() {
	if( socketDescriptor != INVALID && socketDescriptor > 3 ){
		int closeRes = close(socketDescriptor);
		Assertions::check([closeRes](){return closeRes == 0;}, "Closing of socketDescriptor failed");
		socketDescriptor = INVALID;
	}
}

SocketWrapper::~SocketWrapper() {
	closeSocket();
}

void SocketWrapper::shutdownSocket() {
	if( socketDescriptor != INVALID ){
		auto res = shutdown(socketDescriptor, SHUT_RD);
		if( res == -1 ){
			std::cerr << "Warning: shutdown of socket failed " << std::endl;
		}
	}

}
