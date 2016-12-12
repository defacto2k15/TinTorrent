//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_MESSAGERESOURCEREQUEST_H
#define TINTORRENT_MESSAGERESOURCEREQUEST_H


#include <codecvt>
#include "../../Common/InMemoryBuffer.h"
#include "Message.h"
#include "../../ModelEntities/Resource.h"
#include "json.hpp"
#include "MessageType.h"

using json = nlohmann::json;

class MessageResourceRequest : public Message{
	Resource resource;
public:

	MessageResourceRequest(Buffer &buffer){
		const char *inString = (const char *)buffer.getData();
		Assertions::check([inString, &buffer](){ return strlen(inString) < buffer.getSize();},
		                  "MessageResourceRequest deserialization. InString bigger than buffer");
		auto j = json::parse(inString);
		// todo throw other exception - something like Model logic exception
		MessageType type;

		type.parseJson(j["Type"]);
		Assertions::check([&](){ return type==MessageType::RESOURCE_REQUEST;}, "MesageResourceRequest deserialization. Type in wrong");
		resource.parseJson(j["Resource"]);
	}

	MessageResourceRequest(Resource resource) : resource(resource){
	}

	Resource getResource() {
		return resource;
	}

	json toJson(){
		json j;
		j["Type"] = MessageType::RESOURCE_REQUEST.getValue();
		j["Resource"] = resource.toJson();
		return j;
	}

	void serializeTo(Buffer &buffer) override {
		json j = toJson();

		std::string str = j.dump();
		buffer.setData((uint8_t *)str.c_str(), str.length()+1);
	}

	bool operator==(const MessageResourceRequest &other) const{
		return resource == other.resource;
	}
};


#endif //TINTORRENT_MESSAGERESOURCEREQUEST_H
