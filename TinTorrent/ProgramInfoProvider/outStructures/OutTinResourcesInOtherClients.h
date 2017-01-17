//
// Created by defacto on 08.01.17.
//

#ifndef TINTORRENT_OUTTINRESOURCESINOTHERCLIENTS_H
#define TINTORRENT_OUTTINRESOURCESINOTHERCLIENTS_H


#include <ModelEntities/TinAddress.h>
#include <ModelEntities/Resource.h>
#include <ostream>

class OutTinResourcesInOtherClients {
	TinAddress address;
	std::vector<Resource> resources;

public:
	OutTinResourcesInOtherClients(const TinAddress &address, const std::vector<Resource> &resources);

	 TinAddress getAddress() ;

	 std::vector<Resource> getResources() ;

	json toJson();

	friend std::ostream &operator<<(std::ostream &os, const OutTinResourcesInOtherClients &clients);
};


#endif //TINTORRENT_OUTTINRESOURCESINOTHERCLIENTS_H
