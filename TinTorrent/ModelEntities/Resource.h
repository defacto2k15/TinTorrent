//
// Created by defacto on 09.12.16.
//

#ifndef TINTORRENT_RESOURCE_H
#define TINTORRENT_RESOURCE_H

#include <string>
#include "json.hpp"
#include "../Common/StringHelp.h"

using json = nlohmann::json;

class Resource {
	std::wstring resourceName;
	size_t  resourceSize;
public:
	Resource(){}
	Resource(const std::wstring &resourceName, size_t resourceSize);

	const std::wstring &getResourceName() const;

	size_t getResourceSize() const;

	bool operator==(const Resource &other) const {
		return resourceName == other.resourceName &&
	           resourceSize == other.resourceSize;
	}

	json toJson(){
		json j;
		j["Name"] = StringHelp::toUtf8(resourceName);
		j["Size"] = resourceSize;
		return j;
	}

	void parseJson(json j){
		resourceName = StringHelp::toUtf16(j["Name"]);
		resourceSize = j["Size"];
	}
};


#endif //TINTORRENT_RESOURCE_H
