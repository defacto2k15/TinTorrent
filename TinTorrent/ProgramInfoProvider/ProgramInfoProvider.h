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
#include <ProgramInfoProvider/outStructures/OutResourcesBeingDownloaded.h>
#include <ProgramInfoProvider/outStructures/OutClientConnectionInfo.h>
#include <Logging/LogBase.h>

class Kernel;

class ProgramInfoProvider{
	Kernel &kernel;
	LogBase log;
public:
	ProgramInfoProvider( Kernel &kernel );

	std::vector<OutLocalResource> getLocalResources();

	std::vector<OutTinResourcesInOtherClients> getResourcesInOtherClients();

	std::vector<Resource> getResourcesToDownload(); // nie uzywaj

	std::vector<Resource> getRevertedResources();

	std::vector<OutClientConnectionInfo> getConnectionsToOtherServersInfo();

	std::vector<OutServerConnectionInfo> getConnectionsToUsInfo();
	
	void changeResourceBlockState(Resource);

	std::vector<Resource> getResourcesThatCanBeAnnounced();

	void changeResourceAnnouncementState( Resource &resource);

	std::vector<Resource> getResourcesThatCanBeDownloaded();

	void orderResourceDownload(Resource resource);

};

#endif //TINTORRENT_PROGRAMINFOPROVIDER_H
