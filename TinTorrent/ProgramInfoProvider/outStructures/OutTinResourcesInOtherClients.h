//
// Created by defacto on 08.01.17.
//

#ifndef TINTORRENT_OUTTINRESOURCESINOTHERCLIENTS_H
#define TINTORRENT_OUTTINRESOURCESINOTHERCLIENTS_H


#include <ModelEntities/TinAddress.h>
#include <ModelEntities/Resource.h>

class OutTinResourcesInOtherClients {
	TinAddress address;
	std::vector<Resource> resources;

public:
	OutTinResourcesInOtherClients(const TinAddress &address, const std::vector<Resource> &resources);

	 TinAddress getAddress() ;

	 std::vector<Resource> getResources() ;

	json toJson(){
		json j;
		j["address"] = address.toString();
		std::vector<json> resourcesVec;
		for( auto &res : resources){
			resourcesVec.push_back(res.toJson());
		}
		j["resources"] = resourcesVec;
		return j;
	}
};


#endif //TINTORRENT_OUTTINRESOURCESINOTHERCLIENTS_H
