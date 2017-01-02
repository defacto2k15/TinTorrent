//
// Created by defacto on 09.12.16.
//

#ifndef TINTORRENT_RESOURCE_H
#define TINTORRENT_RESOURCE_H

#include <string>
#include <Common/Constants.h>
#include <ostream>
#include "json.hpp"
#include "../Common/StringHelp.h"

using json = nlohmann::json;

class Resource {
	std::wstring resourceName;
	size_t  resourceSize;
public:
	Resource() : resourceName(L"UNSET"), resourceSize(0) {}
	Resource(const std::wstring &resourceName, size_t resourceSize);

	const std::wstring &getResourceName() const;

	size_t getResourceSize() const;

	size_t getSegmentCount() const{
		return (size_t) ceil( double(resourceSize) / double(Constants::segmentSize));
	}

	bool operator==(const Resource &other) const {
		return resourceName == other.resourceName &&
	           resourceSize == other.resourceSize;
	}

	json toJson() const{
		json j;
		j["Name"] = StringHelp::toUtf8(resourceName);
		j["Size"] = resourceSize;
		return j;
	}

	void parseJson(json j){
		resourceName = StringHelp::toUtf16(j["Name"]);
		resourceSize = j["Size"];
	}

	friend std::ostream &operator<<(std::ostream &os, const Resource &resource);
};

struct ResourceCompare
{
	bool operator() (const Resource& lhs, const Resource& rhs) const
	{
		return lhs.getResourceName() < rhs.getResourceName() && lhs.getResourceSize() < rhs.getResourceSize();
	}
};



#endif //TINTORRENT_RESOURCE_H
