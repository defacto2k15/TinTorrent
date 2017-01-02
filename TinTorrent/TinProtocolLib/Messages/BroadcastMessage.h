//
// Created by defacto on 31.12.16.
//

#ifndef TINTORRENT_BROADCASTMESSAGE_H
#define TINTORRENT_BROADCASTMESSAGE_H


#include <ModelEntities/Resource.h>
#include "MessageEnum.h"

class BroadcastMessage {
public:
	class BroadcastType : public MessageEnum{
	public:
		BroadcastType(const std::string &value) : MessageEnum(value){};
		explicit BroadcastType(json jsonValue)  {
			parseJson(jsonValue);
		}
		static BroadcastType ANNOUNCE;
		static BroadcastType REVERT;

		void parseJson(std::string value){
			MessageEnum::parseJson(value, {ANNOUNCE, REVERT});
		}
	};
private:
	std::vector<Resource> resources;
	BroadcastType type;
public:
	BroadcastMessage(std::vector<Resource> resources, BroadcastType type) : resources(resources), type(type){
	}

	explicit BroadcastMessage(json &j) : resources(), type(j["Type"]){
		json resourcesJson = j["Resources"];
		for (json::iterator it = resourcesJson.begin(); it != resourcesJson.end(); ++it) {
			Resource res;
			res.parseJson(*it);
			resources.push_back(res);
		}

	}

	json toJson(){
		std::vector<json> resourcesAsJsonVec;
		for( Resource &resource : resources){
			resourcesAsJsonVec.push_back(resource.toJson());
		}
		json j = {
				{"Type", type.getValue()},
				{"Resources", resourcesAsJsonVec}
		};
		size_t messageSize = j.dump().size();
		Assertions::check(messageSize <= Constants::maxBroadcastMessageSize,
		                  Help::Str("Maximum size of broadcast message is ",Constants::maxBroadcastMessageSize,
		                            " but this message has size of ",messageSize) );
		return j;
	}

	static std::vector<BroadcastMessage> generateMessages( std::vector<Resource> resources, BroadcastType type ){
		size_t bytesNeededForType = 40; //todo, ugly hack but works
		size_t additionalBytesPerResourceEntry = 10; // as above
		std::vector<BroadcastMessage> outVec;
		std::vector<Resource> currentResourcesBuffer;
		size_t currentUsedBytes = bytesNeededForType;
		for( Resource &resource : resources ){
			auto resourceAsJson  = resource.toJson();
			size_t bytesUsedByResource = resourceAsJson.dump().size();
			if( currentUsedBytes + bytesNeededForType > Constants::maxBroadcastMessageSize){
				outVec.push_back(BroadcastMessage(currentResourcesBuffer, type));
				currentResourcesBuffer.clear();
				currentUsedBytes = 0;
			}

			currentUsedBytes += bytesUsedByResource + additionalBytesPerResourceEntry;
			currentResourcesBuffer.push_back(resource);
		}
		if( !currentResourcesBuffer.empty()){
			outVec.push_back(BroadcastMessage(currentResourcesBuffer, type));
		}
		return outVec;
	}

	std::vector<Resource> getResources(){
		return resources;
	}
};


#endif //TINTORRENT_BROADCASTMESSAGE_H
