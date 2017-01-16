//
// Created by defacto on 07.01.17.
//

#ifndef TINTORRENT_SOCKETCOMMUNICATIONEXCEPTION_H
#define TINTORRENT_SOCKETCOMMUNICATIONEXCEPTION_H

#include <stdexcept>
#include <Utils/MyString.h>

class SocketCommunicationException : public std::runtime_error {
public:
	SocketCommunicationException(std::string message ) : std::runtime_error(message){
	}

	SocketCommunicationException(std::exception &e ) : std::runtime_error(Help::Str("Caught exception ",e.what())){
	}
};

#endif //TINTORRENT_SOCKETCOMMUNICATIONEXCEPTION_H
