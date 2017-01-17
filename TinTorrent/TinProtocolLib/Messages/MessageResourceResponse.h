//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_MESSAGERESOURCERESPONSE_H
#define TINTORRENT_MESSAGERESOURCERESPONSE_H


#include "MessageEnum.h"
#include "Message.h"
#include "MessageType.h"

class MessageResourceResponse : public Message{
public:
	class ResourceResponseValue : public MessageEnum{
	public:
		ResourceResponseValue();

		static ResourceResponseValue OK;
		static ResourceResponseValue MISSING;
		static ResourceResponseValue REVERTED;

		bool operator==( const ResourceResponseValue &other )const;

		void parseJson(std::string value);
	private:
		ResourceResponseValue( std::string value);;
	};
private:
	ResourceResponseValue responseValue;
public:
	MessageResourceResponse( ResourceResponseValue responseValue  );

	explicit MessageResourceResponse(json &j);

	ResourceResponseValue getValue();

	json toJson() override;

	static MessageType getMessageType();
};


#endif //TINTORRENT_MESSAGERESOURCERESPONSE_H
