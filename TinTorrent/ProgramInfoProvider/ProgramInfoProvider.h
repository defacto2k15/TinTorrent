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

class Kernel;

class ProgramInfoProvider{
	Kernel &kernel;
public:
	ProgramInfoProvider( Kernel &kernel );

	std::vector<OutLocalResource> getLocalResources();

	std::vector<OutTinResourcesInOtherClients> getResourcesInOtherClients();

	std::vector<Resource> getResourcesToDownload(); // nie uzywaj

	std::vector<Resource> getRevertedResources();

	std::vector<OutClientConnectionInfo> getConnectionsToOtherServersInfo();

	std::vector<OutServerConnectionInfo> getConnectionsToUsInfo();
	
	void changeResourceBlockState(Resource);

	std::vector<Resource> getResourcesThatCanBeAnnounced(){
		return getResourcesToDownload();//todo
	}

	void orderResourceAnnouncement( Resource &resource){
		//todo
	}

	std::vector<Resource> getResourcesThatCanBeDownloaded(){
		return getResourcesToDownload();//todo
	}

	void orderResourceDownload(){
		//todo
	}

	std::vector<OutResourcesBeingDownloaded> getDownloadedResourcesInfo(){
		return {
				{{L"file1.txt", 24424},
				                            {{TinAddress("123.12.22.1", 4444), SegmentRange(0, 3)},
				                                     {TinAddress("55.12.22.1", 4444),  SegmentRange(4, 5)},
				                                     {TinAddress("121.12.1.1", 4444),  SegmentRange(7, 9)}
				                            }},
				{{L"file2.txt", 34424},
				                            {{TinAddress("123.12.22.1", 4444), SegmentRange(0, 1)},
				                                     {TinAddress("53.12.22.1", 4444),  SegmentRange(4, 5)},
				                                     {TinAddress("119.12.1.1", 4444),  SegmentRange(7, 9)}
				                            }}};

	}

};

#endif //TINTORRENT_PROGRAMINFOPROVIDER_H
