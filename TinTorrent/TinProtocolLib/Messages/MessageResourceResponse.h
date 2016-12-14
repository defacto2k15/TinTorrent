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
		ResourceResponseValue() {
		}

		static ResourceResponseValue OK;
		static ResourceResponseValue MISSING;
		static ResourceResponseValue REVERTED;

		bool operator==( const ResourceResponseValue &other )const{
			return MessageEnum::operator==(other);
		}

		void parseJson(std::string value){
			MessageEnum::parseJson(value, {OK, MISSING, REVERTED});
		}
	private:
		ResourceResponseValue( std::string value) : MessageEnum(value){};
	};
private:
	ResourceResponseValue responseValue;
public:
	MessageResourceResponse( ResourceResponseValue responseValue  ) : responseValue(responseValue){
	}


	explicit MessageResourceResponse(json &j) {
		MessageType type;
		type.parseJson(j["Type"]);
		Assertions::check([&](){ return type==MessageType::RESOURCE_RESPONSE;}, "MesageResourceResponse deserialization. Type in wrong");
		responseValue.parseJson(j["ResponseValue"]);
	}

	ResourceResponseValue getValue(){
		return responseValue;
	}

	json toJson() override {
		json j  = {
				{"Type", MessageType::RESOURCE_RESPONSE.getValue()},
				{"ResponseValue", responseValue.getValue()}
		};
		return j;
	}

	static MessageType getMessageType(){
		return MessageType::RESOURCE_RESPONSE;
	}
};


#endif //TINTORRENT_MESSAGERESOURCERESPONSE_H
