//
// Created by defacto on 01.01.17.
//

#include "WorkingDirectoryState.h"

bool WorkingDirectoryState::isDownloaded(const Resource &resource) {
	if( resources.count(resource) == 0 ){
		return false;
	}
	for( auto &state : resources[resource]){
		if(!(state == SegmentState::PRESENT)){
			return false;
		}
	}
	return true;
}

SegmentRange WorkingDirectoryState::allocateSegmentsToDownload(Resource resource) {
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

void WorkingDirectoryState::addResource(Resource resource) {
	createResourceFilledWith(resource, SegmentState::PRESENT);
}

void WorkingDirectoryState::addEmptyResource(Resource resource) {
	createResourceFilledWith(resource, SegmentState::MISSING);
}

void WorkingDirectoryState::init(std::vector<FileInfo> &initialFiles) {
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

bool WorkingDirectoryState::contains(Resource &resource) {
	return resources.count(resource) != 0;
}

void WorkingDirectoryState::deallocateSegmentRange(Resource resource, SegmentRange range) {
	auto stateVec = resources[resource];
	for( auto i = range.getMin(); i < range.getMax(); i++ ){
		stateVec[i] = SegmentState ::MISSING;
	}
}

bool WorkingDirectoryState::removeResource(Resource resource) {
	if( resources.count(resource) != 0){
		resources.erase(resource);
		return true;
	}
	return false;
}

void WorkingDirectoryState::setSegmentsAsDownloaded(Resource resource, SegmentRange range) {
	auto stateVec = resources[resource];
	for( auto i = range.getMin(); i < range.getMax(); i++ ){
		stateVec[i] = SegmentState::PRESENT;
	}
}

std::vector<Resource> WorkingDirectoryState::getDownloadedResources() {
	std::vector<Resource> outVec;
	for( auto &pair : resources){
		if( isDownloaded(pair.first)){
			outVec.push_back(pair.first);
		}
	}
	return outVec;
}

std::vector<OutLocalResource> WorkingDirectoryState::getOutLocalResource() {
	std::vector<OutLocalResource> outVec;
	for( auto &pair : resources ){
		outVec.push_back( OutLocalResource( pair.first, calculatePresencePercent( pair.second)));
	}
	return outVec;
}

uint8_t WorkingDirectoryState::calculatePresencePercent(std::vector<SegmentState> &vec) {
	auto presentSegmentsCount = std::count_if( begin(vec), end(vec), []( SegmentState &state ){ return state == SegmentState ::PRESENT;});
	return (uint8_t )(( 100 * presentSegmentsCount) / vec.size());
}

void WorkingDirectoryState::createResourceFilledWith(Resource resource, SegmentState state) {
	size_t segmentCount = resource.getSegmentCount();
	Assertions::check( segmentCount > 0, "Problem: Calculated segment count is 0");
	std::vector<SegmentState> segmentStateVec(segmentCount);
	for( auto i = 0u; i < segmentCount; i++){
		segmentStateVec[i] =  state;
	}
	resources[resource] = segmentStateVec;
}
