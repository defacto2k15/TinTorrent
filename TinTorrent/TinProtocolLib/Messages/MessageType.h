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

	void parseJson(std::string value){
		MessageEnum::parseJson(value, {RESOURCE_REQUEST});
	}
};


#endif //TINTORRENT_MESSAGETYPE_H
