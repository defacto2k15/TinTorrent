//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_WORKINGDIRECTORYSTATE_H
#define TINTORRENT_WORKINGDIRECTORYSTATE_H


#include <FileManager/ModelEntities/FileInfo.h>
#include <Common/SegmentRange.h>
#include "SegmentState.h"

class WorkingDirectoryState {
	std::map<Resource, std::vector<SegmentState>, ResourceCompare > resources;
public:
	void init( std::vector<FileInfo> &initialFiles ){
		for( FileInfo &file : initialFiles){
			size_t segmentCount = file.getResource().getSegmentCount();
			std::vector<SegmentState> segmentStateVec(segmentCount);
			for( auto i = 0u; i < segmentCount; i++){
				if( file.getFileSegmentsInfo().getSegmentPresence(i)){
					segmentStateVec[i] =  SegmentState::PRESENT;
				} else {
					segmentStateVec[i] = SegmentState::MISSING;
				}
			}
			resources[file.getResource()] = segmentStateVec;
		}
	}

	bool isDownloaded( const Resource &resource) {
		for( auto &state : resources[resource]){
			if(!(state == SegmentState::PRESENT)){
				return false;
			}
		}
		return true;
	}

	bool contains(Resource &resource) {
		return resources.count(resource) != 0;
	}

	void deallocateSegmentRange(Resource resource, SegmentRange range){
		auto stateVec = resources[resource];
		for( auto i = range.getMin(); i < range.getMax(); i++ ){
			stateVec[i] = SegmentState ::MISSING;
		}
	}

	void removeResource(Resource resource) {
		resources.erase(resource);
	}

	void setSegmentsAsDownloaded(Resource resource, SegmentRange range) {
		auto stateVec = resources[resource];
		for( auto i = range.getMin(); i < range.getMax(); i++ ){
			stateVec[i] = SegmentState::PRESENT;
		}
	}

	SegmentRange allocateSegmentsToDownload(Resource resource) {
		auto stateVec = resources[resource];
		unsigned rangeMin = (unsigned)-1;
		unsigned rangeMax = (unsigned)-1;
		for( auto i = 0u; i < stateVec.size(); i++ ){
			if( stateVec[i] == SegmentState::MISSING){
				if( rangeMin == (unsigned)-1){
					rangeMin = i;
				} else 	if( i - rangeMin >= Constants::maxSegmentChunkSize){
					rangeMax = i;
					break;

				} else {
					continue;
				}
			} else{
				if( rangeMin == (unsigned)-1){
					rangeMax = i;
				} else {
					continue;
				}
			}
		}

		if( rangeMin == (unsigned)-1){
			Assertions::fail("There is no segments left to download");
		}
		if( rangeMax == (unsigned)-1){
			rangeMax = (unsigned )stateVec.size();
		}
		return SegmentRange(rangeMin, rangeMax);
	}

	void addResource(Resource resource) {
		size_t segmentCount = resource.getSegmentCount();
		std::vector<SegmentState> segmentStateVec(segmentCount);
		for( auto i = 0u; i < segmentCount; i++){
			segmentStateVec[i] =  SegmentState::PRESENT;
		}
		resources[resource] = segmentStateVec;
	}

	std::vector<Resource> getDownloadedResources(){
		std::vector<Resource> outVec;
		for( auto &pair : resources){
			if( isDownloaded(pair.first)){
				outVec.push_back(pair.first);
			}
		}
		return outVec;
	}
};


#endif //TINTORRENT_WORKINGDIRECTORYSTATE_H
