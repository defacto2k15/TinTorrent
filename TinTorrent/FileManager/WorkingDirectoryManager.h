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
	WorkingDirectoryManager(std::string workingDirectory)
		: log("WorkingDirectoryManager"), workingDirectory(workingDirectory){
	}

	std::vector<FileInfo> check() {
		renameResourcesWithoutSize();
		std::vector<File> files = getAllFilesFromWorkingDirectory();

		std::vector< std::pair<File, File> > outVec;
		for( auto &file : files ){
			if( !file.isMetadata()){
				log.debug(": We see ",file.name," as resource file");
				std::string metadataName = file.getMetadataName();
				std::experimental::optional<File> metadataFile
						= ContainerUtils::GetElem<File>(files, [metadataName](File &file){ return file.name == metadataName;} );
				if(metadataFile){
					outVec.push_back(std::make_pair(file, metadataFile.value()));
					log.debug(": Found ", metadataFile->name," as metadata file");
				} else {
					File mf = createMetadataFile(file);
					outVec.push_back(std::make_pair(file, mf));
					log.debug(": Created metadata file");
				}
			} else {
				log.debug(":: We see ",file.name," as metadata file");
				std::string resourceFileName = file.getResourceName();
				std::experimental::optional<File> resourceFile
						= ContainerUtils::GetElem<File>(files,[resourceFileName](File &file){ return file.name == resourceFileName;});
				if( resourceFile ){
					log.debug(": There is resource file for it");
					//do nothing, is ok
				} else {
					log.debug(": There is no resource file for it, removing metadata");
					removeFile( file);
				}
			}
		}
		return generateFileInfos( outVec );
	}

	SegmentsSet getSegments(Resource resource, SegmentRange range){
		auto resourceFile = createFileFromResource(resource);
		std::string fileName = resourceFile.fullPath;
		std::vector<Segment> segmentsVec;
		for( auto segmentIndex = range.getMin(); segmentIndex < range.getMax(); segmentIndex++){
			auto buffer = std::make_shared<InMemoryBuffer>(Constants::segmentSize);
			size_t readBytes = FileUtils::readContent( fileName, *buffer, (size_t)segmentIndex  * Constants::segmentSize, Constants::segmentSize);
			Segment segment(buffer, SegmentInfo((uint16_t)segmentIndex, readBytes));
			segmentsVec.push_back(segment);
		}
		return  SegmentsSet(range, segmentsVec);
	}

	void setSegments( Resource resource, SegmentsSet segmentsSet ){
		auto resourceFile = createFileFromResource(resource);
		std::string fileName = resourceFile.fullPath;
		for( auto segmentIndex = segmentsSet.getRange().getMin(); segmentIndex < segmentsSet.getRange().getMax(); segmentIndex++){
			Segment segment = segmentsSet.getSegment(segmentIndex);
			FileUtils::writeContent(fileName, segment.getPayload(), segmentIndex*Constants::segmentSize);
		}
	}

	void removeResource( Resource &resourceToRemove){
		auto resourceFile = createFileFromResource(resourceToRemove);
		FileUtils::removeFile(resourceFile.fullPath);
		FileUtils::removeFile(resourceFile.getMetadataPath());
	}


	void addNewResource( Resource &resource ){
		auto resourceFile = createFileFromResource(resource);
		FileUtils::createFile(resourceFile.fullPath, resourceFile.size);
		FileUtils::createZeroedFile(resourceFile.getMetadataPath(), SegmentUtils::SegmentBytesCount(resourceFile.size));
	}


private:
	std::vector<File> getAllFilesFromWorkingDirectory(){
		DIR *dir;
		struct dirent *ent;
		dir = opendir (workingDirectory.c_str());
		Assertions::check(dir != NULL, Help::Str("Working directory of'",workingDirectory,"' cannot be opened"));
		std::vector<File> outVec;
		while( (ent = readdir(dir)) != NULL){
			log.debug(": Found file of name ", ent->d_name, " type ", ent->d_type);
			if( ent ->d_type == DT_REG ){
				std::string fullPath = workingDirectory+"/"+ent->d_name;
				struct stat st;
				stat(fullPath.c_str(), &st);
				outVec.push_back(File(ent->d_name, fullPath, st.st_size));
				log.debug(": It is regular file, can process, size is ",st.st_size);
			}
		}
		closedir(dir);

		return outVec;
	}

	File createMetadataFile( File &resourceFile ){
		auto metadataPath = resourceFile.getMetadataPath();
		std::ofstream ofs(metadataPath, std::ios::binary | std::ios::out);
		size_t segmentsCount = (size_t)ceil( double(resourceFile.size) / double(Constants::segmentSize));
		size_t fileSize = StreamUtils::writeToStream(ofs, std::vector<bool>(segmentsCount, true));
		return File(FileUtils::getFilenameFromPath(metadataPath), metadataPath, fileSize);
	}

	void removeFile( File &file ){
		std::remove(file.fullPath.c_str());
	}

	std::vector<FileInfo> generateFileInfos( std::vector< std::pair<File, File>> &files){
		std::vector<FileInfo> fileInfos;
		for( auto pair : files ){
			fileInfos.push_back(createFileInfo(pair.first, pair.second));
		}
		return fileInfos;
	}

	FileInfo createFileInfo( File &resourceFile, File &metadataFile ){
		std::string resourceNameWithoutSizeSuffix = resourceFile.getResourceNameWithoutSizeSuffix();
		Resource resource( StringHelp::toUtf16(resourceNameWithoutSizeSuffix), resourceFile.size);
		FileSegmentsInfo segmentsInfo( SegmentUtils::SegmentCount( resourceFile.size), metadataFile.fullPath);
		return FileInfo(resource, segmentsInfo);
	}

	void renameResourcesWithoutSize(){
		std::vector<File> files = getAllFilesFromWorkingDirectory();

		for( auto &file : files ){
			if( file.isResourceWithoutProperName() ){
				file.renameFileToProperResourceName();
			}
		}
	}

	File createFileFromResource(Resource &resource){
		auto filename = Help::Str( StringHelp::toUtf8(resource.getResourceName()),".", resource.getResourceSize());
		return File (filename, Help::Str(workingDirectory, "/", filename), resource.getResourceSize());
	}

};


#endif //TINTORRENT_WORKINGDIRECTORYMANAGER_H
