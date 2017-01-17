//
// Created by defacto on 08.12.16.
//

#include "MessageStartSendingRequest.h"
MessageStartSendingRequest::PreviousStatus MessageStartSendingRequest::PreviousStatus::OK(std::string("Ok"));
MessageStartSendingRequest::PreviousStatus MessageStartSendingRequest::PreviousStatus::FIRST(std::string("First"));
MessageStartSendingRequest::PreviousStatus MessageStartSendingRequest::PreviousStatus::WRONG_SEGMENT(std::string("WrongSegment"));
MessageStartSendingRequest::PreviousStatus MessageStartSendingRequest::PreviousStatus::WRONG_SIZE(std::string("WrongSize"));

MessageStartSendingRequest::PreviousStatus::PreviousStatus(const std::string &value) : MessageEnum(value){}

MessageStartSendingRequest::PreviousStatus::PreviousStatus(json jsonValue) {
	parseJson(jsonValue);
}

void MessageStartSendingRequest::PreviousStatus::parseJson(std::string value) {
	MessageEnum::parseJson(value, {OK, FIRST, WRONG_SEGMENT, WRONG_SIZE});
}

MessageStartSendingRequest::MessageStartSendingRequest(json &j) : previous(j["Previous"]), segmentInfo(j["SegmentInfo"]){
	MessageType type;
	type.parseJson(j["Type"]);
	Assertions::check([&](){ return type==MessageType::START_SENDING_REQUEST;}, "StartSendingRequest deserialization. Type in wrong");
}

MessageStartSendingRequest::MessageStartSendingRequest(SegmentInfo segment,
                                                       MessageStartSendingRequest::PreviousStatus status) :previous(status), segmentInfo(segment) {
}

json MessageStartSendingRequest::toJson() {
	json j  = {
			{"Type", MessageType::START_SENDING_REQUEST.getValue()},
			{"Previous", previous.getValue()},
			{"SegmentInfo", segmentInfo.toJson()}
	};
	return j;
}

MessageStartSendingRequest::PreviousStatus MessageStartSendingRequest::getPreviousStatus() {
	return previous;
}

SegmentInfo MessageStartSendingRequest::getSegmentInfo() {
	return segmentInfo;
}

bool MessageStartSendingRequest::operator==(const MessageStartSendingRequest &other) const {
	return this->segmentInfo == other.segmentInfo &&
	       this->previous == other.previous;
}

MessageType MessageStartSendingRequest::getMessageType() {
	return MessageType::START_SENDING_REQUEST;
}
