//
// Created by defacto on 07.01.17.
//

#ifndef TINTORRENT_PROGRAMINFOPROVIDER_H
#define TINTORRENT_PROGRAMINFOPROVIDER_H

#include <ProgramInfoProvider/outStructures/OutLocalResource.h>
#include <Multithreading/BlockingElement.h>
#include <future>
#include <ProgramInfoProvider/outStructures/OutTinResourcesInOtherClients.h>
#include <experimental/optional>
#include <ProgramInfoProvider/outStructures/OutServerConnectionInfo.h>

class Kernel;

class ProgramInfoProvider{
	Kernel &kernel;
public:
	ProgramInfoProvider( Kernel &kernel );

	std::vector<OutLocalResource> getLocalResources();

	std::vector<OutTinResourcesInOtherClients> getResourcesInOtherClients();

	std::vector<Resource> getResourcesToDownload();

	std::vector<Resource> getRevertedResources();

	std::vector<OutClientConnectionInfo> getConnectionsToOtherServersInfo();

	std::vector<OutServerConnectionInfo> getConnectionsToUsInfo();
	
	void changeResourceBlockState(Resource);
};

#endif //TINTORRENT_PROGRAMINFOPROVIDER_H
