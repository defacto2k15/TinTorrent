//
// Created by defacto on 12.12.16.
//

#ifndef TINTORRENT_MESSAGETYPE_H
#define TINTORRENT_MESSAGETYPE_H


#include "MessageEnum.h"

class MessageType : public MessageEnum {
private:
	MessageType( std::string value) : MessageEnum(value){};
public:
	MessageType(){}
	static MessageType RESOURCE_REQUEST;
	static MessageType RESOURCE_RESPONSE;
	static MessageType START_SENDING_REQUEST;
	static MessageType CLOSE;

	void parseJson(std::string value){
		MessageEnum::parseJson(value, {RESOURCE_RESPONSE, RESOURCE_REQUEST, START_SENDING_REQUEST, CLOSE});
	}
};


#endif //TINTORRENT_MESSAGETYPE_H
