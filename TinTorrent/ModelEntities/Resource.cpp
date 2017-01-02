//
// Created by defacto on 09.12.16.
//

#include "Resource.h"

Resource::Resource(const std::wstring &resourceName, size_t resourceSize) : resourceName(resourceName),
                                                                            resourceSize(resourceSize) {}

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
