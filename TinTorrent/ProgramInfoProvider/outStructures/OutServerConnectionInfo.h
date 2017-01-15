//
// Created by defacto on 08.01.17.
//

#ifndef TINTORRENT_OUTSERVERCONNECTIONINFO_H
#define TINTORRENT_OUTSERVERCONNECTIONINFO_H


#include <ModelEntities/TinAddress.h>
#include <ModelEntities/Resource.h>
#include <ostream>

class OutServerConnectionInfo {
	TinAddress address;
	Resource resource;
public:
	OutServerConnectionInfo(const TinAddress &address, const Resource &resource);

	const TinAddress &getAddress() const;

	const Resource &getResource() const;

	json toJson() {
		json j;
		j["address"] = address.toString();
		j["resource"] = resource.toJson();
		return j;
	}

	friend std::ostream &operator<<(std::ostream &os, const OutServerConnectionInfo &info);
};


#endif //TINTORRENT_OUTSERVERCONNECTIONINFO_H
