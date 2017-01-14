//
// Created by defacto on 14.01.17.
//

#ifndef TINTORRENT_LOCALRESOURCESSTATEINFO_H
#define TINTORRENT_LOCALRESOURCESSTATEINFO_H


#include <Utils/ContainerUtils.h>
#include "WorkingDirectoryState.h"

class  LocalResourcesStateInfo
{
	WorkingDirectoryState workingDirectoryState;
	std::vector<Resource> revertedResources;
	std::vector<Resource> resourcesToDownload;
protected:
	LocalResourcesStateInfo(LocalResourcesStateInfo const &) = delete;
	void operator=(LocalResourcesStateInfo const &x) = delete;
public:
	LocalResourcesStateInfo() = default;

	void init(std::vector<FileInfo> fileInfos ){
		workingDirectoryState.init(fileInfos);
	}

	bool isReverted( Resource resource ){
		return ContainerUtils::Contains(revertedResources, resource);
	}

	void addLocalResource( Resource resource ){
		workingDirectoryState.addResource(resource);
	}

	void addEmptyLocalResource( Resource resource ){
		workingDirectoryState.addEmptyResource(resource);
	}

	bool isDownloaded( Resource resource ){
		return workingDirectoryState.isDownloaded(resource);
	}

	void setToDownload( Resource resource ){
		ContainerUtils::addWithoutDuplicate(resourcesToDownload, resource);
	}

	bool containsLocalResource( Resource resource ){
		return workingDirectoryState.contains(resource);
	}

	void deallocateSegmentRange( Resource resource, SegmentRange range ){
		workingDirectoryState.deallocateSegmentRange(resource, range);
	}

	void setSegmentsAsDownloaded( Resource resource, SegmentRange range ){
		workingDirectoryState.setSegmentsAsDownloaded(resource, range);
	}

	SegmentRange allocateSegmentsToDownload( Resource resource ){
		return workingDirectoryState.allocateSegmentsToDownload(resource);
	}

	void setAsDownloaded( Resource resource ){
		ContainerUtils::addWithoutDuplicate(resourcesToDownload, resource);
	}

	void removeLocalResource( Resource resource ){
		workingDirectoryState.removeResource(resource);
		ContainerUtils::remove(resourcesToDownload, resource);
	}

	std::vector<Resource> getDownloadedResources(){
		return workingDirectoryState.getDownloadedResources();
	}

	std::vector<Resource> getReverted(){
		return revertedResources;
	}

	std::vector<Resource> getResourcesToDownload(){
		return resourcesToDownload;
	}

	WorkingDirectoryState& getWorkingDirectoryState(){
		return workingDirectoryState;
	}

	bool isLocalResourcePresent( Resource resource ){
		return workingDirectoryState.contains( resource);
	}

	void setAsReverted(Resource resource ){
		workingDirectoryState.removeResource(resource);
		ContainerUtils::remove(resourcesToDownload, resource);
		ContainerUtils::addWithoutDuplicate(revertedResources, resource);
	}

};


#endif //TINTORRENT_LOCALRESOURCESSTATEINFO_H
