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

	explicit MessageResourceRequest(json &j);

	MessageResourceRequest(const Resource &resource);

	Resource getResource();

	json toJson() override;

	bool operator==(const MessageResourceRequest &other) const;

	static MessageType getMessageType();
};


#endif //TINTORRENT_MESSAGERESOURCEREQUEST_H
