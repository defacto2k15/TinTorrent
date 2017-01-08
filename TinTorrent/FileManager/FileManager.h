//
// Created by defacto on 26.12.16.
//

#ifndef TINTORRENT_FILEMANAGER_H
#define TINTORRENT_FILEMANAGER_H


#include <Common/SegmentsSet.h>
#include <Common/SegmentRange.h>
#include <Common/Exceptions/MissingRequiredFile.h>
#include "WorkingDirectoryManager.h"

class FileManager {
	WorkingDirectoryManager manager;
	std::vector<FileInfo> workingDirectoryState;
public:
	FileManager(std::string workingDirectory);

	std::vector<FileInfo> initialCheck();

	UpdateInfo update();

	SegmentsSet getSegments(Resource requiredResource, SegmentRange segmentRange);

	void setSegments( Resource requiredResource, SegmentsSet segments);

	void removeResource( Resource resourceToRemove );

	void createEmptyResource ( Resource newResource );

private:
	std::vector<FileInfo> findRemovedFiles( std::vector<FileInfo> &newFileInfos);

	std::vector<FileInfo> findAddedFiles( std::vector<FileInfo> &newFileInfos);

	void assertResourceIsPresent( Resource &requiredResource);

	void assertResourceIsNotPresent( Resource &requiredResource);

	void removeResourceInfo( Resource &resource );

	void addResourceInfo( Resource &resource );

	void setSegmentsAsPresent(  Resource &requiredResource, const SegmentRange range );

};


#endif //TINTORRENT_FILEMANAGER_H
