//
// Created by defacto on 08.12.16.
//

#include "MessageResourceRequest.h"

MessageResourceRequest::MessageResourceRequest(json &j) {
	MessageType type;
	type.parseJson(j["Type"]);
	Assertions::check([&](){ return type==MessageType::RESOURCE_REQUEST;}, "MesageResourceRequest deserialization. Type in wrong");
	resource.parseJson(j["Resource"]);
}

MessageResourceRequest::MessageResourceRequest(const Resource &resource) : resource(resource){
}

Resource MessageResourceRequest::getResource() {
	return resource;
}

json MessageResourceRequest::toJson() {
	json j;
	j["Type"] = MessageType::RESOURCE_REQUEST.getValue();
	j["Resource"] = resource.toJson();
	return j;
}

bool MessageResourceRequest::operator==(const MessageResourceRequest &other) const {
	return resource == other.resource;
}

MessageType MessageResourceRequest::getMessageType() {
	return MessageType::RESOURCE_REQUEST;
}
