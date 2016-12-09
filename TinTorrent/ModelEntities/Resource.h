//
// Created by defacto on 09.12.16.
//

#ifndef TINTORRENT_RESOURCE_H
#define TINTORRENT_RESOURCE_H

#include <string>

class Resource {
	std::wstring resourceName;
	size_t  resourceSize;
public:
	Resource(){}
	Resource(const std::wstring &resourceName, size_t resourceSize);

	const std::wstring &getResourceName() const;

	size_t getResourceSize() const;
};


#endif //TINTORRENT_RESOURCE_H
