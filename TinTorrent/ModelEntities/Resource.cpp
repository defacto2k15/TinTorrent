//
// Created by defacto on 09.12.16.
//

#include "Resource.h"

Resource::Resource(const std::wstring &resourceName, size_t resourceSize) : resourceName(resourceName),
                                                                            resourceSize(resourceSize),
																									 blocked(false) {}

const std::wstring &Resource::getResourceName() const {
	return resourceName;
}

size_t Resource::getResourceSize() const {
	return resourceSize;
}

std::ostream &operator<<(std::ostream &os, const Resource &resource) {
	os << resource.toJson().dump();
	return os;
}

Resource::Resource() : resourceName(L"UNSET"), resourceSize(0) {}

size_t Resource::getSegmentCount() const {
	return (size_t) ceil( double(resourceSize) / double(Constants::segmentSize));
}

bool Resource::operator==(const Resource &other) const {
	return resourceName == other.resourceName &&
	       resourceSize == other.resourceSize;
}

json Resource::toJson() const {
	json j;
	j["Name"] = StringHelp::toUtf8(resourceName);
	j["Size"] = resourceSize;
	return j;
}

void Resource::parseJson(json j) {
	resourceName = StringHelp::toUtf16(j["Name"]);
	resourceSize = j["Size"];
}

void Resource::setBlockState(bool blocked) {
	this->blocked = blocked;	
}


bool Resource::isBlocked() {
	return blocked;
}
