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
	bool blocked;
public:
	Resource();
	Resource(const std::wstring &resourceName, size_t resourceSize);

	const std::wstring &getResourceName() const;

	size_t getResourceSize() const;

	size_t getSegmentCount() const;

	bool operator==(const Resource &other) const;

	json toJson() const;

	void parseJson(json j);

	friend std::ostream &operator<<(std::ostream &os, const Resource &resource);

	void setBlockState(bool);

	bool isBlocked();
};

struct ResourceCompare
{
	bool operator() (const Resource& lhs, const Resource& rhs) const
	{
		return lhs.getResourceName() < rhs.getResourceName() || lhs.getResourceSize() < rhs.getResourceSize();
	}
};



#endif //TINTORRENT_RESOURCE_H
