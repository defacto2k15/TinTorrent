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
	FileManager(std::string workingDirectory) : manager(workingDirectory){
	}

	std::vector<FileInfo> initialCheck(){
		workingDirectoryState = manager.check();
		return workingDirectoryState;
	}

	UpdateInfo update(){
		auto newWorkingDirectoryState = manager.check();
		auto removedFiles = findRemovedFiles( newWorkingDirectoryState );
		auto addedFiles = findAddedFiles ( newWorkingDirectoryState );
		workingDirectoryState = newWorkingDirectoryState;
		return UpdateInfo(addedFiles, removedFiles);
	}

	SegmentsSet getSegments(Resource requiredResource, SegmentRange segmentRange){
		assertResourceIsPresent(requiredResource);
		return manager.getSegments( requiredResource, segmentRange);
	}

	void setSegments( Resource requiredResource, SegmentsSet segments){
		assertResourceIsPresent(requiredResource);
		setSegmentsAsPresent( requiredResource, segments.getRange());
		manager.setSegments( requiredResource, segments);
	}

	void removeResource( Resource resourceToRemove ){
		assertResourceIsPresent( resourceToRemove);
		removeResourceInfo( resourceToRemove );
		manager.removeResource( resourceToRemove );
	}

	void createEmptyResource ( Resource newResource ){
		assertResourceIsNotPresent(  newResource );
		addResourceInfo( newResource );
		manager.addNewResource( newResource );
	}

private:
	std::vector<FileInfo> findRemovedFiles( std::vector<FileInfo> &newFileInfos){
		return  ContainerUtils::getDifference<FileInfo>(workingDirectoryState, newFileInfos, []( FileInfo &f1, FileInfo &f2 ){
			return f1.getResource() == f2.getResource();
		});
	}

	std::vector<FileInfo> findAddedFiles( std::vector<FileInfo> &newFileInfos){
		return  ContainerUtils::getDifference<FileInfo>( newFileInfos, workingDirectoryState, []( FileInfo &f1, FileInfo &f2 ){
			return f1.getResource() == f2.getResource();
		});
	}

	void assertResourceIsPresent( Resource &requiredResource){
		auto requiredFileInfo = ContainerUtils::GetElem<FileInfo>(workingDirectoryState, [&requiredResource](FileInfo &fileInfo){
			return requiredResource == fileInfo.getResource();
		});
		if( !requiredFileInfo){
			throw MissingRequiredFile(Help::Str("Required resource ",requiredResource.toJson()," not found in FileManager's workingDirectory records"));
		}
	}

	void assertResourceIsNotPresent( Resource &requiredResource){
		auto requiredFileInfo = ContainerUtils::GetElem<FileInfo>(workingDirectoryState, [&requiredResource](FileInfo &fileInfo){
			return requiredResource == fileInfo.getResource();
		});
		if( requiredFileInfo){
			throw MissingRequiredFile(Help::Str("New resource ",requiredResource.toJson()," arleady present in FileManager's workingDirectory records"));
		}
	}

	void removeResourceInfo( Resource &resource ){
		workingDirectoryState.erase(
			std::remove_if(workingDirectoryState.begin(), workingDirectoryState.end(), [&resource](FileInfo &info){
				return info.getResource() == resource;
			}),
			workingDirectoryState.end());
	}

	void addResourceInfo( Resource &resource ){
		workingDirectoryState.push_back(FileInfo(resource, FileSegmentsInfo(resource.getSegmentCount())));
	}

	void setSegmentsAsPresent(  Resource &requiredResource, const SegmentRange range ){
		auto fileInfo = *(ContainerUtils::GetElem<FileInfo>(workingDirectoryState, [&requiredResource]( FileInfo &info){
			return info.getResource() == requiredResource;
		}));
		fileInfo.getFileSegmentsInfo();
		for(auto i = 0u; i < SegmentUtils::SegmentCount(requiredResource.getResourceSize()); i++ ){
			if( range.inRange(i)){
				fileInfo.getFileSegmentsInfo().setSegmentPresence(i, true);
			}
		}
	}

};


#endif //TINTORRENT_FILEMANAGER_H
