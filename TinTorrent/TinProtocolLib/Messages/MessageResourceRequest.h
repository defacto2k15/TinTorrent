//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_MESSAGERESOURCEREQUEST_H
#define TINTORRENT_MESSAGERESOURCEREQUEST_H

#include "../../Common/InMemoryBuffer.h"
#include "Message.h"
#include "../../ModelEntities/Resource.h"
#include "json.hpp"
#include "MessageType.h"

using json = nlohmann::json;

class MessageResourceRequest : public Message{
	Resource resource;
public:

	explicit MessageResourceRequest(json &j){
		MessageType type;
		type.parseJson(j["Type"]);
		Assertions::check([&](){ return type==MessageType::RESOURCE_REQUEST;}, "MesageResourceRequest deserialization. Type in wrong");
		resource.parseJson(j["Resource"]);
	}

	MessageResourceRequest(const Resource &resource) : resource(resource){
	}

	Resource getResource() {
		return resource;
	}

	json toJson() override {
		json j;
		j["Type"] = MessageType::RESOURCE_REQUEST.getValue();
		j["Resource"] = resource.toJson();
		return j;
	}

	bool operator==(const MessageResourceRequest &other) const{
		return resource == other.resource;
	}

	static MessageType getMessageType(){
		return MessageType::RESOURCE_REQUEST;
	}
};


#endif //TINTORRENT_MESSAGERESOURCEREQUEST_H
