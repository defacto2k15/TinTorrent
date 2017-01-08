//
// Created by defacto on 21.12.16.
//

#ifndef TINTORRENT_WORKINGDIRECTORYMANAGER_H
#define TINTORRENT_WORKINGDIRECTORYMANAGER_H

#include "Common/Constants.h"
#include <dirent.h>
#include "../Test/Mocks/MockFileInformationConsumer.h"
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

		File(const std::string &name, const std::string &fullPath, size_t size) :
				log("File"), name(name), fullPath(fullPath), size(size) {
		}

		bool isMetadata(){
			return StringHelp::Matches(name, Constants::metadataNameRegexp);
		}

		bool hasFileSizeSuffix(){
			std::string expectedFileSuffix = Help::Str(".", size);
			return StringHelp::EndsWith(name, expectedFileSuffix);
		}

		std::string getMetadataName(){
			Assertions::check( !isMetadata(), Help::Str("File of name ", name, " is arleady metadata"));
			return Help::Str(name,  Constants::metadataFileSuffix);
		}

		std::string getMetadataPath(){
			Assertions::check( !isMetadata(), Help::Str("File of name ", name, " is arleady metadata"));
			return Help::Str(fullPath, Constants::metadataFileSuffix);
		}

		std::string getResourceName(){
			Assertions::check( isMetadata(), Help::Str("File of name ", name, " is not metadata"));
			return StringHelp::RemoveSuffix( name, getMetadataSuffix());
		}

		void renameFileToProperResourceName(){
			std::string newFullPath = Help::Str(fullPath, ".", size);
			log.debug(": Renaming file to proper resource name. From: ",fullPath, " to ", newFullPath);
			FileUtils::renameFile(fullPath, newFullPath);
			fullPath = newFullPath;
			name = Help::Str(name, ".", size);
		}

		bool isResourceWithoutProperName(){
			return !isMetadata() && !hasFileSizeSuffix();
		}

		bool isResource(){
			return !isMetadata() && hasProperFileSuffix();
		}

		bool hasProperFileSuffix(){
			if( !hasFileSizeSuffix()){
				return false;
			}
			auto sizeInName = std::atoi(StringHelp::MatchesAndGetLastMatchGroup(name, Constants::resourceNameRegexp).c_str());
			return size == (size_t)sizeInName;
		}

		std::string getResourceNameWithoutSizeSuffix(){
			//Assertions::check( hasProperFileSuffix(), Help::Str("File of name ", name, " dont have proper filesize suffix"));
			return StringHelp::removeSuffixAfter( name, ".");
		}

		~File(){
		}
	private:
		std::string getMetadataSuffix(){
			return Help::Str( Constants::metadataFileSuffix);
		}
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
