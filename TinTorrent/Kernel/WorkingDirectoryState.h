//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_WORKINGDIRECTORYSTATE_H
#define TINTORRENT_WORKINGDIRECTORYSTATE_H


#include <FileManager/ModelEntities/FileInfo.h>
#include "SegmentState.h"

class WorkingDirectoryState {
	std::map<Resource, std::vector<SegmentState> > resources;
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
};


#endif //TINTORRENT_WORKINGDIRECTORYSTATE_H
