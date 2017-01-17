//
// Created by defacto on 08.12.16.
//

#include "MessageResourceResponse.h"


MessageResourceResponse::ResourceResponseValue MessageResourceResponse::ResourceResponseValue::OK("Ok");
MessageResourceResponse::ResourceResponseValue MessageResourceResponse::ResourceResponseValue::MISSING("Missing");
MessageResourceResponse::ResourceResponseValue MessageResourceResponse::ResourceResponseValue::REVERTED("Reverted");

MessageResourceResponse::ResourceResponseValue::ResourceResponseValue() {
}

bool MessageResourceResponse::ResourceResponseValue::operator==(
		const MessageResourceResponse::ResourceResponseValue &other) const {
	return MessageEnum::operator==(other);
}

void MessageResourceResponse::ResourceResponseValue::parseJson(std::string value) {
	MessageEnum::parseJson(value, {OK, MISSING, REVERTED});
}

MessageResourceResponse::ResourceResponseValue::ResourceResponseValue(std::string value) : MessageEnum(value){}

MessageResourceResponse::MessageResourceResponse(MessageResourceResponse::ResourceResponseValue responseValue) : responseValue(responseValue){
}

MessageResourceResponse::MessageResourceResponse(json &j) {
	MessageType type;
	type.parseJson(j["Type"]);
	Assertions::check([&](){ return type==MessageType::RESOURCE_RESPONSE;}, "MesageResourceResponse deserialization. Type in wrong");
	responseValue.parseJson(j["ResponseValue"]);
}

MessageResourceResponse::ResourceResponseValue MessageResourceResponse::getValue() {
	return responseValue;
}

json MessageResourceResponse::toJson() {
	json j  = {
			{"Type", MessageType::RESOURCE_RESPONSE.getValue()},
			{"ResponseValue", responseValue.getValue()}
	};
	return j;
}

MessageType MessageResourceResponse::getMessageType() {
	return MessageType::RESOURCE_RESPONSE;
}
