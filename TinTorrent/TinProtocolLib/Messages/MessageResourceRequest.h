//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_MESSAGERESOURCEREQUEST_H
#define TINTORRENT_MESSAGERESOURCEREQUEST_H


#include <codecvt>
#include "../../Common/InMemoryBuffer.h"
#include "Message.h"
#include "../../ModelEntities/Resource.h"

class MessageResourceRequest : public Message{
	Resource resource;
public:

	MessageResourceRequest(InMemoryBuffer buffer){
		std::string tempName(buffer.getData(), buffer.getData()+4);
		std::wstring name (tempName.begin(), tempName.end());

		size_t length = *(buffer.getData()+5);
		resource = Resource(name, length);
	}
};


#endif //TINTORRENT_MESSAGERESOURCEREQUEST_H
