//
// Created by defacto on 08.12.16.
//

#include "MessageClose.h"

MessageClose::CloseReason::CloseReason(const std::string &value) : MessageEnum(value) {}

MessageClose::MessageClose(MessageClose::CloseReason reason) {

}

void MessageClose::serializeTo(Buffer &buffer) {
	//todo
}
