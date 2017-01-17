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

	void addFiles(TinAddress address, std::vector<Resource> resources);

	void addResource (TinAddress &address, Resource &resource);

	void addAddress(TinAddress &address);

	void removeResourceFromClient(TinAddress address, Resource resource);

	void removeClient(TinAddress &address);

	void removeResource(Resource &resource);

	std::vector<TinAddress> avalibleClientsForDownload(Resource &resource);

	std::vector<OutTinResourcesInOtherClients> getResourcesInOtherClients();
};


#endif //TINTORRENT_TINNETWORKSTATE_H
