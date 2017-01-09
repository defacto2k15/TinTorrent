//
// Created by defacto on 31.12.16.
//

#ifndef TINTORRENT_BROADCASTMESSAGE_H
#define TINTORRENT_BROADCASTMESSAGE_H


#include <ModelEntities/Resource.h>
#include <ostream>
#include "MessageEnum.h"

class BroadcastMessage {
public:
	class BroadcastType : public MessageEnum{
	public:
		BroadcastType(const std::string &value);;
		explicit BroadcastType(json jsonValue);
		static BroadcastType ANNOUNCE;
		static BroadcastType REVERT;

		void parseJson(std::string value);
	};
private:
	std::vector<Resource> resources;
	BroadcastType type;
public:
	BroadcastMessage(std::vector<Resource> resources, BroadcastType type);

	explicit BroadcastMessage(json &j);

	json toJson();

	static std::vector<BroadcastMessage> generateMessages( std::vector<Resource> resources, BroadcastType type );

	std::vector<Resource> getResources();

	BroadcastType getType();

	friend std::ostream &operator<<(std::ostream &os, const BroadcastMessage &message);
};


#endif //TINTORRENT_BROADCASTMESSAGE_H
