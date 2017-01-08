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
