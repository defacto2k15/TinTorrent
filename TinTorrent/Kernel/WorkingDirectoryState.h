//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_WORKINGDIRECTORYSTATE_H
#define TINTORRENT_WORKINGDIRECTORYSTATE_H


#include <FileManager/ModelEntities/FileInfo.h>
#include <Common/SegmentRange.h>
#include <ProgramInfoProvider/outStructures/OutLocalResource.h>
#include "SegmentState.h"

class WorkingDirectoryState {
	std::map<Resource, std::vector<SegmentState>, ResourceCompare > resources;
public:
	void init( std::vector<FileInfo> &initialFiles );

	bool isDownloaded( const Resource resource);

	bool contains(Resource resource);

	void deallocateSegmentRange(Resource resource, SegmentRange range);

	bool removeResource(Resource resource);

	void setSegmentsAsDownloaded(Resource resource, SegmentRange range);

	SegmentRange allocateSegmentsToDownload(Resource resource);

	void addResource(Resource resource);

	void addEmptyResource(Resource resource);

	std::vector<Resource> getDownloadedResources();

	std::vector<OutLocalResource> getOutLocalResource();

private:
	uint8_t calculatePresencePercent( std::vector<SegmentState> &vec );

	void createResourceFilledWith( Resource resource, SegmentState state );
};


#endif //TINTORRENT_WORKINGDIRECTORYSTATE_H
