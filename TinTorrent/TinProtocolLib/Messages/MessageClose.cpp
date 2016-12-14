//
// Created by defacto on 08.12.16.
//
#include "MessageType.h"
#include "MessageClose.h"

MessageClose::CloseReason MessageClose::CloseReason::OK("Ok");

MessageClose::CloseReason::CloseReason(const std::string &value) : MessageEnum(value) {}

MessageClose::CloseReason::CloseReason() : MessageEnum(""){}

MessageClose::MessageClose(MessageClose::CloseReason reason) :reason(reason){
}

json MessageClose::toJson() {
	return json {
			{"Type", MessageType::CLOSE.getValue()},
			{"Reason", reason.getValue()}
	};
}

MessageClose::MessageClose(json &j)  {
	MessageType type;
	type.parseJson(j["Type"]);
	Assertions::check([&](){ return type==MessageType::CLOSE;}, "CloseMessage deserialization. Type in wrong");
	reason.parseJson(j["Reason"]);
}

bool MessageClose::operator==(const MessageClose &rhs) const {
	return reason == rhs.reason;
}

bool MessageClose::operator!=(const MessageClose &rhs) const {
	return !(rhs == *this);
}
