//
// Created by defacto on 07.01.17.
//

#ifndef TINTORRENT_SOCKETCOMMUNICATIONEXCEPTION_H
#define TINTORRENT_SOCKETCOMMUNICATIONEXCEPTION_H

#include <stdexcept>
#include <Utils/MyString.h>

class SocketCommunicationException : public std::runtime_error {
public:
	SocketCommunicationException(std::string message );

	SocketCommunicationException(std::exception &e );
};

#endif //TINTORRENT_SOCKETCOMMUNICATIONEXCEPTION_H
