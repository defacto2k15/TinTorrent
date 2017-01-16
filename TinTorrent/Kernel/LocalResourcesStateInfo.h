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

	WorkingDirectoryState workingDirectoryState;
	std::map<Resource, ResourceState, ResourceCompare> resourceStates;
	LogBase log;
protected:
	LocalResourcesStateInfo(LocalResourcesStateInfo const &) = delete;
	void operator=(LocalResourcesStateInfo const &x) = delete;
public:
	LocalResourcesStateInfo() : log("LocalResourcesStateInfo"){
	}

	void init(std::vector<FileInfo> fileInfos ){
		workingDirectoryState.init(fileInfos);
		for( auto &res : workingDirectoryState.getAllResources()){
			if( workingDirectoryState.isDownloaded(res) ){
				if( Constants::automaticAnnouncement ){
					resourceStates[res] = ResourceState::DOWNLOADED_BROADCASTABLE;
				} else {
					resourceStates[res] = ResourceState::DOWNLOADED_NONBROADCASTABLE;
				}

			} else {
				if( Constants::automaticDownload ){
					resourceStates[res] = ResourceState ::CAN_BE_DOWNLOADED;
				} else {
					resourceStates[res] = ResourceState ::NOT_INTRESTING;
				}
			}
		}
	}

	bool isReverted( Resource resource ){
		if( resourceStates.count(resource) == 0 ){
			return false;
		}
		return resourceStates[resource] == ResourceState::REVERTED;
	}

	void addNewlyFoundLocalResource( Resource resource ){
		if( Constants::automaticAnnouncement ){
			resourceStates[resource] = ResourceState::DOWNLOADED_BROADCASTABLE;
		} else {
			resourceStates[resource] = ResourceState::DOWNLOADED_NONBROADCASTABLE;
		}
		workingDirectoryState.addResource(resource);
	}

	void addEmptyLocalResource( Resource resource ){
		resourceStates[resource] = ResourceState ::CAN_BE_DOWNLOADED;
		workingDirectoryState.addEmptyResource(resource);
	}

	bool isDownloaded( Resource resource ){
		return workingDirectoryState.isDownloaded(resource);
	}

	void setToDownload( Resource resource ){
		if( (resourceStates[resource] != ResourceState::DOWNLOADED_BROADCASTABLE  )
		    && (resourceStates[resource] != ResourceState::DOWNLOADED_NONBROADCASTABLE  )
		    && (resourceStates[resource] != ResourceState::REVERTED)){
			resourceStates[resource] = ResourceState ::CAN_BE_DOWNLOADED;
		}
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
		if( Constants::automaticAnnouncement ){
			resourceStates[resource] = ResourceState::DOWNLOADED_BROADCASTABLE;
		} else {
			resourceStates[resource] = ResourceState::DOWNLOADED_NONBROADCASTABLE;
		}
	}

	void removeLocalResource( Resource resource ){
		resourceStates.erase(resource);
		workingDirectoryState.removeResource(resource);
	}

	std::vector<Resource> getDownloadedResources(){
		return workingDirectoryState.getDownloadedResources();
	}

	std::vector<Resource> getReverted(){
		std::vector<Resource> outVec;
		for( auto &pair : resourceStates ){
			if( pair.second == ResourceState::REVERTED){
				outVec.push_back(pair.first);
			}
		}
		return outVec;
	}

	std::vector<Resource> getResourcesToDownload(){
		std::vector<Resource> outVec;
		for( auto &pair : resourceStates ){
			if( pair.second == ResourceState::CAN_BE_DOWNLOADED){
				outVec.push_back(pair.first);
			}
		}
		return outVec;
	}

	std::vector<Resource> getBroadcastableResources(){
		std::vector<Resource> outVec;
		for( auto &pair : resourceStates ){
			if( pair.second == ResourceState::DOWNLOADED_BROADCASTABLE){
				outVec.push_back(pair.first);
			}
		}
		return outVec;
	}

	WorkingDirectoryState& getWorkingDirectoryState(){
		return workingDirectoryState;
	}

	bool isLocalResourcePresent( Resource resource ){
		return workingDirectoryState.contains( resource);
	}

	void setAsReverted(Resource resource ){
		workingDirectoryState.removeResource(resource);
		resourceStates[resource] = ResourceState ::REVERTED;
	}

	void changeResourceAnnouncementState(Resource resource ){
		if( resourceStates.count(resource ) == 0){
			log.warn(Help::Str("Soft Error. There is no info about resource ",resource," so i cant change announcement"));
		}
		if( resourceStates[resource] == DOWNLOADED_BROADCASTABLE ){
			resourceStates[resource] = DOWNLOADED_NONBROADCASTABLE;
		} else if(resourceStates[resource] == DOWNLOADED_NONBROADCASTABLE  ){
			resourceStates[resource] = DOWNLOADED_BROADCASTABLE;
		} else {
			log.warn(Help::Str("Soft Error. Resource ",resource," has state ", resourceStates[resource], " so i cant change announcement"));
		}

	}

	void changeResourceDownloadabilityState(Resource resource ){
		if( resourceStates.count(resource ) == 0){
			log.warn(Help::Str("Soft Error. There is no info about resource ",resource," so i cant change Downloadability"));
		}
		if( resourceStates[resource] == NOT_INTRESTING ){
			resourceStates[resource] = CAN_BE_DOWNLOADED;
		} else if(resourceStates[resource] == CAN_BE_DOWNLOADED  ){
			resourceStates[resource] = NOT_INTRESTING;
		} else {
			log.warn(Help::Str("Soft Error. Resource ",resource," has state ", resourceStates[resource], " so i cant change Downloadability"));
		}

	}

	void addEmptyLocalResourceNotDoDownload(Resource &res){
		resourceStates[res] = ResourceState ::NOT_INTRESTING;
	}

};


#endif //TINTORRENT_LOCALRESOURCESSTATEINFO_H
