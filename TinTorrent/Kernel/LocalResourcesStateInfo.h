//
// Created by defacto on 14.01.17.
//

#ifndef TINTORRENT_LOCALRESOURCESSTATEINFO_H
#define TINTORRENT_LOCALRESOURCESSTATEINFO_H


#include <Utils/ContainerUtils.h>
#include <Logging/LogBase.h>
#include "WorkingDirectoryState.h"

class  LocalResourcesStateInfo
{
private:
	enum ResourceState {
		ERROR_DONTUSE, REVERTED, NOT_INTRESTING, CAN_BE_DOWNLOADED, DOWNLOADED_BROADCASTABLE, DOWNLOADED_NONBROADCASTABLE
	};

	std::string resourceStateToString(ResourceState state);

	WorkingDirectoryState workingDirectoryState;
	std::map<Resource, ResourceState, ResourceCompare> resourceStates;
	LogBase log;
protected:
	LocalResourcesStateInfo(LocalResourcesStateInfo const &) = delete;
	void operator=(LocalResourcesStateInfo const &x) = delete;
public:
	LocalResourcesStateInfo();

	void init(std::vector<FileInfo> fileInfos );

	bool isReverted( Resource resource );

	void addNewlyFoundLocalResource( Resource resource );

	void addEmptyLocalResource( Resource resource );

	bool isDownloaded( Resource resource );

	void setToDownload( Resource resource );

	bool containsLocalResource( Resource resource );

	void deallocateSegmentRange( Resource resource, SegmentRange range );

	void setSegmentsAsDownloaded( Resource resource, SegmentRange range );

	SegmentRange allocateSegmentsToDownload( Resource resource );

	void setAsDownloaded( Resource resource );

	void removeLocalResource( Resource resource );

	std::vector<Resource> getDownloadedResources();

	std::vector<Resource> getReverted();

	std::vector<Resource> getResourcesToDownload();

	std::vector<Resource> getBroadcastableResources();

	std::vector<Resource> getResourcesThatAreNotYetDownloaded();

	WorkingDirectoryState& getWorkingDirectoryState();

	bool isLocalResourcePresent( Resource resource );

	void setAsReverted(Resource resource );

	void changeResourceAnnouncementState(Resource resource );

	void changeResourceDownloadabilityState(Resource resource );

	void addEmptyLocalResourceNotDoDownload(Resource &res);

	void writeResourceStatesInfoToLog();
};

#endif //TINTORRENT_LOCALRESOURCESSTATEINFO_H
