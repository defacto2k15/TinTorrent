//
// Created by defacto on 12.12.16.
//

#include "MessageType.h"

MessageType MessageType::RESOURCE_REQUEST("ResourceRequest");
MessageType MessageType::RESOURCE_RESPONSE("ResourceResponse");
MessageType MessageType::START_SENDING_REQUEST("StartSendingRequest");
MessageType MessageType::CLOSE("Close");

MessageType::MessageType(std::string value) : MessageEnum(value){}

MessageType::MessageType() {}

void MessageType::parseJson(std::string value) {
	MessageEnum::parseJson(value, {RESOURCE_RESPONSE, RESOURCE_REQUEST, START_SENDING_REQUEST, CLOSE});
}
