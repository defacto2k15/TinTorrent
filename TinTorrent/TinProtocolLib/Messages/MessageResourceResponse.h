//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_MESSAGERESOURCERESPONSE_H
#define TINTORRENT_MESSAGERESOURCERESPONSE_H


#include "MessageEnum.h"
#include "Message.h"

class MessageResourceResponse : public Message{
public:
	class ResourceResponseValue : MessageEnum{
	public:
		static ResourceResponseValue OK;
		static ResourceResponseValue MISSING;
		static ResourceResponseValue REVERTED;
	private:
		ResourceResponseValue( std::string value) : MessageEnum(value){};
	};

	MessageResourceResponse(ResourceResponseValue value);

	MessageResourceResponse(Buffer &buffer);

	ResourceResponseValue getValue();

	void serializeTo(Buffer &buffer) override;
};


#endif //TINTORRENT_MESSAGERESOURCERESPONSE_H
