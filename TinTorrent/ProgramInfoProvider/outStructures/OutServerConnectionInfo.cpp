//
// Created by defacto on 08.01.17.
//

#include "OutServerConnectionInfo.h"

OutServerConnectionInfo::OutServerConnectionInfo(const TinAddress &address, const Resource &resource) : address(
		address), resource(resource) {}

const TinAddress &OutServerConnectionInfo::getAddress() const {
	return address;
}

const Resource &OutServerConnectionInfo::getResource() const {
	return resource;
}

std::ostream &operator<<(std::ostream &os, const OutServerConnectionInfo &info) {
	auto otherInfo = info;
	os << otherInfo.toJson().dump();
	return os;
}

json OutServerConnectionInfo::toJson() {
	json j;
	j["address"] = address.toString();
	j["resource"] = resource.toJson();
	return j;
}
