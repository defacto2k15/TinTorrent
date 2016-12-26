//
// Created by defacto on 21.12.16.
//

#ifndef TINTORRENT_FILEMANAGER_H
#define TINTORRENT_FILEMANAGER_H

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


class FileManager {
	std::string workingDirectory;
	std::vector<FileInfo> fileInfos;

	class File {
	public:
		std::string name;
		std::string fullPath;
		size_t size;

		File(const std::string &name, const std::string &fullPath, size_t size) :
				name(name), fullPath(fullPath), size(size) {

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
			std::cout << Help::Str("Renaming file to proper resource name. From: ",fullPath, " to ", newFullPath) << std::endl;
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
			return size == sizeInName;
		}

		~File(){
		}
	private:
		std::string getMetadataSuffix(){
			return Help::Str( Constants::metadataFileSuffix);
		}
	};
public:
	FileManager(std::string workingDirectory)
		: workingDirectory(workingDirectory){
	}

	std::vector<FileInfo> initialCheck() {
		renameResourcesWithoutSize();
		std::vector<File> files = getAllFilesFromWorkingDirectory();

		std::vector< std::pair<File, File> > outVec;
		for( auto &file : files ){
			if( !file.isMetadata()){
				std::cout << Help::Str("We see ",file.name," as resource file") << std::endl;
				std::string metadataName = file.getMetadataName();
				std::experimental::optional<File> metadataFile
						= ContainerUtils::GetElem<File>(files, [metadataName](File &file){ return file.name == metadataName;} );
				if(metadataFile){
					outVec.push_back(std::make_pair(file, metadataFile.value()));
					std::cout << Help::Str("Found ", metadataFile->name," as metadata file") << std::endl;
				} else {
					File mf = createMetadataFile(file);
					outVec.push_back(std::make_pair(file, mf));
					std::cout << Help::Str("Created metadata file") << std::endl;
				}
			} else {
				std::cout << Help::Str("We see ",file.name," as metadata file") << std::endl;
				std::string resourceFileName = file.getResourceName();
				std::experimental::optional<File> resourceFile
						= ContainerUtils::GetElem<File>(files,[resourceFileName](File &file){ return file.name == resourceFileName;});
				if( resourceFile ){
					std::cout << Help::Str("There is resource file for it") << std::endl;
					//do nothing, is ok
				} else {
					std::cout << Help::Str("There is no resource file for it, removing metadata") << std::endl;
					removeFile( file);
				}
			}
		}
		saveFileInfos( outVec );
		return fileInfos;
	}

//	UpdateInfo update(){
//		std::vector<File> files = getAllFilesFromWorkingDirectory();
//		std::vector<FileInfo> removedFiles;
//		std::vector<FileInfo> newFiles;
//		std::vector<FileInfo> updatedFileInfoList;
//
//		for( auto &file : files ){
//			if( file.isResourceWithoutProperName() ){
//				file.renameFileToProperResourceName();
//			}
//		}
//
//		for( auto &fileInfo : fileInfos ){
//			auto foundFile = ContainerUtils::GetElem(files, [&fileInfo](File &file){
//				return file.name == StringHelp::toUtf8(fileInfo.getResource().getResourceName());
//			});
//			if( !foundFile ){
//				removedFiles.push_back(fileInfo);
//			}else {
//				updatedFileInfoList.push_back(fileInfo);
//			}
//		}
//
//		fileInfos = updatedFileInfoList;
//
//		for( auto &file : files ){
//			if( file.isResource()){
//				auto foundFile = ContainerUtils::GetElem( fileInfos, [&file]( FileInfo &info){
//					return file.matchesResource( info);
//				});
//
//				if( !foundFile){
//
//				}
//			}
//		}
//
//
//		return UpdateInfo(newFiles, removedFiles);
//	}

private:
	std::vector<File> getAllFilesFromWorkingDirectory(){
		DIR *dir;
		struct dirent *ent;
		dir = opendir (workingDirectory.c_str());
		Assertions::check(dir != NULL, Help::Str("Working directory of'",workingDirectory,"' cannot be opened"));
		std::vector<File> outVec;
		while( (ent = readdir(dir)) != NULL){
			std::cout << Help::Str("Found file of name ", ent->d_name, " type ", ent->d_type) << std::endl;
			if( ent ->d_type == DT_REG ){
				std::string fullPath = workingDirectory+"/"+ent->d_name;
				struct stat st;
				stat(fullPath.c_str(), &st);
				outVec.push_back(File(ent->d_name, fullPath, st.st_size));
				std::cout << Help::Str("It is regular file, can process, size is ",st.st_size) << std::endl;
			}
		}
		closedir(dir);

		return outVec;
	}

	File createMetadataFile( File &resourceFile ){
		auto metadataPath = resourceFile.getMetadataPath();
		std::ofstream ofs(metadataPath, std::ios::binary | std::ios::out);
		size_t segmentsCount = ceil( double(resourceFile.size) / double(Constants::segmentSize));
		size_t fileSize = StreamUtils::writeToStream(ofs, std::vector<bool>(segmentsCount, true));
		return File(FileUtils::getFilenameFromPath(metadataPath), metadataPath, fileSize);
	}

	void removeFile( File &file ){
		std::remove(file.fullPath.c_str());
	}

	void saveFileInfos( std::vector< std::pair<File, File>> &files){
		for( auto pair : files ){
			fileInfos.push_back(createFileInfo(pair.first, pair.second));
		}
	}

	FileInfo createFileInfo( File &resourceFile, File &metadataFile ){
		Resource resource( StringHelp::toUtf16(resourceFile.name), resourceFile.size);
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
};


#endif //TINTORRENT_FILEMANAGER_H
