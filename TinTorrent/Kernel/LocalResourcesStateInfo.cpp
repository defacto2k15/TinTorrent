//
// Created by defacto on 14.01.17.
//

#include "LocalResourcesStateInfo.h"

std::string LocalResourcesStateInfo::resourceStateToString(LocalResourcesStateInfo::ResourceState state) {
	switch(state){
		case ERROR_DONTUSE:
			return "ERROR_DONTUSE";
		case REVERTED:
			return "REVERTED";
		case NOT_INTRESTING:
			return "NOT_INTRESTING";
		case CAN_BE_DOWNLOADED:
			return "CAN_BE_DOWNLOADED";
		case DOWNLOADED_BROADCASTABLE:
			return "DOWNLOADED_BROADCASTABLE";
		case DOWNLOADED_NONBROADCASTABLE:
			return "DOWNLOADED_NONBROADCASTABLE";
		default:
			return "DEFAULT VALUE";
	}
}

LocalResourcesStateInfo::LocalResourcesStateInfo() : log("LocalResourcesStateInfo"){
}

void LocalResourcesStateInfo::init(std::vector<FileInfo> fileInfos) {
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

bool LocalResourcesStateInfo::isReverted(Resource resource) {
	if( resourceStates.count(resource) == 0 ){
		return false;
	}
	return resourceStates[resource] == ResourceState::REVERTED;
}

void LocalResourcesStateInfo::addNewlyFoundLocalResource(Resource resource) {
	if( Constants::automaticAnnouncement ){
		resourceStates[resource] = ResourceState::DOWNLOADED_BROADCASTABLE;
	} else {
		resourceStates[resource] = ResourceState::DOWNLOADED_NONBROADCASTABLE;
	}
	workingDirectoryState.addResource(resource);
}

void LocalResourcesStateInfo::addEmptyLocalResource(Resource resource) {
	resourceStates[resource] = ResourceState ::CAN_BE_DOWNLOADED;
	workingDirectoryState.addEmptyResource(resource);
}

bool LocalResourcesStateInfo::isDownloaded(Resource resource) {
	return workingDirectoryState.isDownloaded(resource);
}

void LocalResourcesStateInfo::setToDownload(Resource resource) {
	if( (resourceStates[resource] != ResourceState::DOWNLOADED_BROADCASTABLE  )
	    && (resourceStates[resource] != ResourceState::DOWNLOADED_NONBROADCASTABLE  )
	    && (resourceStates[resource] != ResourceState::REVERTED)){
		resourceStates[resource] = ResourceState ::CAN_BE_DOWNLOADED;
	}
}

bool LocalResourcesStateInfo::containsLocalResource(Resource resource) {
	return workingDirectoryState.contains(resource);
}

void LocalResourcesStateInfo::deallocateSegmentRange(Resource resource, SegmentRange range) {
	workingDirectoryState.deallocateSegmentRange(resource, range);
}

void LocalResourcesStateInfo::setSegmentsAsDownloaded(Resource resource, SegmentRange range) {
	workingDirectoryState.setSegmentsAsDownloaded(resource, range);
}

SegmentRange LocalResourcesStateInfo::allocateSegmentsToDownload(Resource resource) {
	return workingDirectoryState.allocateSegmentsToDownload(resource);
}

void LocalResourcesStateInfo::setAsDownloaded(Resource resource) {
	if( Constants::automaticAnnouncement ){
		resourceStates[resource] = ResourceState::DOWNLOADED_BROADCASTABLE;
	} else {
		resourceStates[resource] = ResourceState::DOWNLOADED_NONBROADCASTABLE;
	}
}

void LocalResourcesStateInfo::removeLocalResource(Resource resource) {
	resourceStates.erase(resource);
	workingDirectoryState.removeResource(resource);
}

std::vector<Resource> LocalResourcesStateInfo::getDownloadedResources() {
	return workingDirectoryState.getDownloadedResources();
}

std::vector<Resource> LocalResourcesStateInfo::getReverted() {
	std::vector<Resource> outVec;
	for( auto &pair : resourceStates ){
		if( pair.second == ResourceState::REVERTED){
			outVec.push_back(pair.first);
		}
	}
	return outVec;
}

std::vector<Resource> LocalResourcesStateInfo::getResourcesToDownload() {
	std::vector<Resource> outVec;
	for( auto &pair : resourceStates ){
		if( pair.second == ResourceState::CAN_BE_DOWNLOADED){
			outVec.push_back(pair.first);
		}
	}
	return outVec;
}

std::vector<Resource> LocalResourcesStateInfo::getBroadcastableResources() {
	std::vector<Resource> outVec;
	for( auto &pair : resourceStates ){
		if( pair.second == ResourceState::DOWNLOADED_BROADCASTABLE){
			outVec.push_back(pair.first);
		}
	}
	return outVec;
}

std::vector<Resource> LocalResourcesStateInfo::getResourcesThatAreNotYetDownloaded() {
	std::vector<Resource> outVec;
	for( auto &pair : resourceStates ){
		if( pair.second == ResourceState::NOT_INTRESTING){
			outVec.push_back(pair.first);
		}
	}
	return outVec;
}

WorkingDirectoryState &LocalResourcesStateInfo::getWorkingDirectoryState() {
	return workingDirectoryState;
}

bool LocalResourcesStateInfo::isLocalResourcePresent(Resource resource) {
	return workingDirectoryState.contains( resource);
}

void LocalResourcesStateInfo::setAsReverted(Resource resource) {
	workingDirectoryState.removeResource(resource);
	resourceStates[resource] = ResourceState ::REVERTED;
}

void LocalResourcesStateInfo::changeResourceAnnouncementState(Resource resource) {
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

void LocalResourcesStateInfo::changeResourceDownloadabilityState(Resource resource) {
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

void LocalResourcesStateInfo::addEmptyLocalResourceNotDoDownload(Resource &res) {
	resourceStates[res] = ResourceState ::NOT_INTRESTING;
}

void LocalResourcesStateInfo::writeResourceStatesInfoToLog() {
	for( auto &pair : resourceStates ){
		log.warn(Help::Str("[",pair.first.toJson().dump(),"] = ", resourceStateToString(pair.second)));
	}
}
