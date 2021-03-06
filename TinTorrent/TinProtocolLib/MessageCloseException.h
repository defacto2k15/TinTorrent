//
// Created by defacto on 13.12.16.
//

#ifndef TINTORRENT_MESSAGECLOSEEXCEPTION_H
#define TINTORRENT_MESSAGECLOSEEXCEPTION_H


#include <stdexcept>
#include "Messages/MessageClose.h"

class MessageCloseException : public std::runtime_error{
	MessageClose messageClose;
public:
	MessageCloseException(MessageClose &messageClose);

	MessageClose getMessageClose();
};


#endif //TINTORRENT_MESSAGECLOSEEXCEPTION_H
