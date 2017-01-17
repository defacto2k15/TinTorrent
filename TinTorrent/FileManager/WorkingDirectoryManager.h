//
// Created by defacto on 21.12.16.
//

#ifndef TINTORRENT_WORKINGDIRECTORYMANAGER_H
#define TINTORRENT_WORKINGDIRECTORYMANAGER_H

#include "Common/Constants.h"
#include <dirent.h>
#include "../Assertions/Assertions.h"
#include "../Utils/MyString.h"
#include "UpdateInfo.h"
#include <sys/stat.h>
#include <experimental/optional>
#include <Utils/ContainerUtils.h>
#include <fstream>
#include <Utils/StreamUtils.h>
#include <Utils/SegmentUtils.h>
#include <Utils/FileUtils.h>
#include <Common/SegmentsSet.h>
#include <Common/SegmentRange.h>
#include <Common/InMemoryBuffer.h>
#include <Logging/LogBase.h>


class WorkingDirectoryManager {
	LogBase log;
	std::string workingDirectory;

	class File {
	public:
		LogBase log;
		std::string name;
		std::string fullPath;
		size_t size;

		File(const std::string &name, const std::string &fullPath, size_t size);

		bool isMetadata();

		bool hasFileSizeSuffix();

		std::string getMetadataName();

		std::string getMetadataPath();

		std::string getResourceName();

		void renameFileToProperResourceName();

		bool isResourceWithoutProperName();

		bool isResource();

		bool hasProperFileSuffix();

		std::string getResourceNameWithoutSizeSuffix();

		~File();
	private:
		std::string getMetadataSuffix();
	};
public:
	WorkingDirectoryManager(std::string workingDirectory);

	std::vector<FileInfo> check();

	SegmentsSet getSegments(Resource resource, SegmentRange range);

	void setSegments( Resource resource, SegmentsSet segmentsSet );

	void removeResource( Resource &resourceToRemove);


	void addNewResource( Resource &resource );


private:
	std::vector<File> getAllFilesFromWorkingDirectory();

	File createMetadataFile( File &resourceFile );

	void removeFile( File &file );

	std::vector<FileInfo> generateFileInfos( std::vector< std::pair<File, File>> &files);

	FileInfo createFileInfo( File &resourceFile, File &metadataFile );

	void renameResourcesWithoutSize();

	File createFileFromResource(Resource &resource);

};


#endif //TINTORRENT_WORKINGDIRECTORYMANAGER_H
