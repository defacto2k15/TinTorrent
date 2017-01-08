//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_TINNETWORKSTATE_H
#define TINTORRENT_TINNETWORKSTATE_H


#include <ModelEntities/TinAddress.h>
#include <TinProtocolLib/Messages/BroadcastMessage.h>
#include <Utils/ContainerUtils.h>
#include <ProgramInfoProvider/outStructures/OutTinResourcesInOtherClients.h>

class TinNetworkState {
	std::map<TinAddress, std::vector<Resource>, TinAddressCompare > resourceMap;
public:

	void addFiles(TinAddress address, std::vector<Resource> resources){
		for( auto &resource : resources){
			if( !ContainerUtils::Contains(resourceMap[address], resource)){
				resourceMap[address].push_back(resource);
			}
		}
	}

	void removeResourceFromClient(TinAddress address, Resource resource) {
		ContainerUtils::remove(resourceMap[address], resource);
	}

	void removeClient(TinAddress &address) {
		resourceMap.erase(address);
	}

	void removeResource(Resource &resource) {
		for( auto &pair : resourceMap){
			ContainerUtils::remove(pair.second, resource);
		}
	}

	std::vector<TinAddress> avalibleClientsForDownload(Resource &resource) {
		std::vector<TinAddress> outVec;
		for( auto & pair : resourceMap){
			if( ContainerUtils::Contains(pair.second, resource)){
				outVec.push_back(pair.first);
			}
		}
		return outVec;
	}

	std::vector<OutTinResourcesInOtherClients> getResourcesInOtherClients(){
		std::vector<OutTinResourcesInOtherClients> outVec;
		for( auto &pair : resourceMap){
			outVec.push_back( OutTinResourcesInOtherClients(pair.first, pair.second));
		}
		return outVec;
	}
};


#endif //TINTORRENT_TINNETWORKSTATE_H
