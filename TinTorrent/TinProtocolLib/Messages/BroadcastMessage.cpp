//
// Created by defacto on 31.12.16.
//

#include "BroadcastMessage.h"

BroadcastMessage::BroadcastType BroadcastMessage::BroadcastType::ANNOUNCE(std::string("Announce"));
BroadcastMessage::BroadcastType BroadcastMessage::BroadcastType::REVERT(std::string("Revert"));

BroadcastMessage::BroadcastType::BroadcastType(const std::string &value) : MessageEnum(value){}

BroadcastMessage::BroadcastType::BroadcastType(json jsonValue) {
	parseJson(jsonValue);
}

void BroadcastMessage::BroadcastType::parseJson(std::string value) {
	MessageEnum::parseJson(value, {ANNOUNCE, REVERT});
}

BroadcastMessage::BroadcastMessage(std::vector<Resource> resources, BroadcastMessage::BroadcastType type) : resources(resources), type(type){
}

BroadcastMessage::BroadcastMessage(json &j) : resources(), type(j["Type"]){
	json resourcesJson = j["Resources"];
	for (json::iterator it = resourcesJson.begin(); it != resourcesJson.end(); ++it) {
		Resource res;
		res.parseJson(*it);
		resources.push_back(res);
	}

}

json BroadcastMessage::toJson() {
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

std::vector<BroadcastMessage>
BroadcastMessage::generateMessages(std::vector<Resource> resources, BroadcastMessage::BroadcastType type) {
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

std::vector<Resource> BroadcastMessage::getResources() {
	return resources;
}

BroadcastMessage::BroadcastType BroadcastMessage::getType() {
	return type;
}

std::ostream &operator<<(std::ostream &os, const BroadcastMessage &message) {
	os << "resources: " << Help::writeVecContents(message.resources) << " type: " << message.type.getValue();
	return os;
}
