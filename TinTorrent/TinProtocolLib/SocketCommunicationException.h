//
// Created by defacto on 07.01.17.
//

#ifndef TINTORRENT_SOCKETCOMMUNICATIONEXCEPTION_H
#define TINTORRENT_SOCKETCOMMUNICATIONEXCEPTION_H

#include <stdexcept>

class SocketCommunicationException : public std::runtime_error {
public:
	SocketCommunicationException(std::string message ) : std::runtime_error(message){
	}
};

#endif //TINTORRENT_SOCKETCOMMUNICATIONEXCEPTION_H
