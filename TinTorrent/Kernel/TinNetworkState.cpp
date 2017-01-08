//
// Created by defacto on 01.01.17.
//

#include "TinNetworkState.h"

void TinNetworkState::addFiles(TinAddress address, std::vector<Resource> resources) {
	for( auto &resource : resources){
		if( !ContainerUtils::Contains(resourceMap[address], resource)){
			resourceMap[address].push_back(resource);
		}
	}
}

void TinNetworkState::removeResourceFromClient(TinAddress address, Resource resource) {
	ContainerUtils::remove(resourceMap[address], resource);
}

void TinNetworkState::removeClient(TinAddress &address) {
	resourceMap.erase(address);
}

void TinNetworkState::removeResource(Resource &resource) {
	for( auto &pair : resourceMap){
		ContainerUtils::remove(pair.second, resource);
	}
}

std::vector<TinAddress> TinNetworkState::avalibleClientsForDownload(Resource &resource) {
	std::vector<TinAddress> outVec;
	for( auto & pair : resourceMap){
		if( ContainerUtils::Contains(pair.second, resource)){
			outVec.push_back(pair.first);
		}
	}
	return outVec;
}

std::vector<OutTinResourcesInOtherClients> TinNetworkState::getResourcesInOtherClients() {
	std::vector<OutTinResourcesInOtherClients> outVec;
	for( auto &pair : resourceMap){
		outVec.push_back( OutTinResourcesInOtherClients(pair.first, pair.second));
	}
	return outVec;
}
