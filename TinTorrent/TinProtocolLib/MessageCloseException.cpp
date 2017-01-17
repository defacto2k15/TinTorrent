//
// Created by defacto on 13.12.16.
//

#include "MessageCloseException.h"

MessageCloseException::MessageCloseException(MessageClose &messageClose) :  std::runtime_error("MessageClose"), messageClose(messageClose){
}

MessageClose MessageCloseException::getMessageClose() {
	return messageClose;
}
