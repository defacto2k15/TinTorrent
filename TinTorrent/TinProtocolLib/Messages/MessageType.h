//
// Created by defacto on 12.12.16.
//

#ifndef TINTORRENT_MESSAGETYPE_H
#define TINTORRENT_MESSAGETYPE_H


#include "MessageEnum.h"

class MessageType : public MessageEnum {
private:
	MessageType( std::string value);;
public:
	MessageType();
	static MessageType RESOURCE_REQUEST;
	static MessageType RESOURCE_RESPONSE;
	static MessageType START_SENDING_REQUEST;
	static MessageType CLOSE;

	void parseJson(std::string value);
};


#endif //TINTORRENT_MESSAGETYPE_H
