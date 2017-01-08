//
// Created by defacto on 21.12.16.
//

#include "WorkingDirectoryManager.h"

WorkingDirectoryManager::WorkingDirectoryManager(std::string workingDirectory)
		: log("WorkingDirectoryManager"), workingDirectory(workingDirectory){
}

std::vector<FileInfo> WorkingDirectoryManager::check() {
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

SegmentsSet WorkingDirectoryManager::getSegments(Resource resource, SegmentRange range) {
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

void WorkingDirectoryManager::setSegments(Resource resource, SegmentsSet segmentsSet) {
	auto resourceFile = createFileFromResource(resource);
	std::string fileName = resourceFile.fullPath;
	for( auto segmentIndex = segmentsSet.getRange().getMin(); segmentIndex < segmentsSet.getRange().getMax(); segmentIndex++){
		Segment segment = segmentsSet.getSegment(segmentIndex);
		FileUtils::writeContent(fileName, segment.getPayload(), segmentIndex*Constants::segmentSize);
	}
}

void WorkingDirectoryManager::removeResource(Resource &resourceToRemove) {
	auto resourceFile = createFileFromResource(resourceToRemove);
	FileUtils::removeFile(resourceFile.fullPath);
	FileUtils::removeFile(resourceFile.getMetadataPath());
}

void WorkingDirectoryManager::addNewResource(Resource &resource) {
	auto resourceFile = createFileFromResource(resource);
	FileUtils::createFile(resourceFile.fullPath, resourceFile.size);
	FileUtils::createZeroedFile(resourceFile.getMetadataPath(), SegmentUtils::SegmentBytesCount(resourceFile.size));
}

std::vector<WorkingDirectoryManager::File> WorkingDirectoryManager::getAllFilesFromWorkingDirectory() {
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

WorkingDirectoryManager::File WorkingDirectoryManager::createMetadataFile(WorkingDirectoryManager::File &resourceFile) {
	auto metadataPath = resourceFile.getMetadataPath();
	std::ofstream ofs(metadataPath, std::ios::binary | std::ios::out);
	size_t segmentsCount = (size_t)ceil( double(resourceFile.size) / double(Constants::segmentSize));
	size_t fileSize = StreamUtils::writeToStream(ofs, std::vector<bool>(segmentsCount, true));
	return File(FileUtils::getFilenameFromPath(metadataPath), metadataPath, fileSize);
}

void WorkingDirectoryManager::removeFile(WorkingDirectoryManager::File &file) {
	std::remove(file.fullPath.c_str());
}

std::vector<FileInfo> WorkingDirectoryManager::generateFileInfos(
		std::vector<std::pair<WorkingDirectoryManager::File, WorkingDirectoryManager::File>> &files) {
	std::vector<FileInfo> fileInfos;
	for( auto pair : files ){
		fileInfos.push_back(createFileInfo(pair.first, pair.second));
	}
	return fileInfos;
}

FileInfo WorkingDirectoryManager::createFileInfo(WorkingDirectoryManager::File &resourceFile,
                                                 WorkingDirectoryManager::File &metadataFile) {
	std::string resourceNameWithoutSizeSuffix = resourceFile.getResourceNameWithoutSizeSuffix();
	Resource resource( StringHelp::toUtf16(resourceNameWithoutSizeSuffix), resourceFile.size);
	FileSegmentsInfo segmentsInfo( SegmentUtils::SegmentCount( resourceFile.size), metadataFile.fullPath);
	return FileInfo(resource, segmentsInfo);
}

void WorkingDirectoryManager::renameResourcesWithoutSize() {
	std::vector<File> files = getAllFilesFromWorkingDirectory();

	for( auto &file : files ){
		if( file.isResourceWithoutProperName() ){
			file.renameFileToProperResourceName();
		}
	}
}

WorkingDirectoryManager::File WorkingDirectoryManager::createFileFromResource(Resource &resource) {
	auto filename = Help::Str( StringHelp::toUtf8(resource.getResourceName()),".", resource.getResourceSize());
	return File (filename, Help::Str(workingDirectory, "/", filename), resource.getResourceSize());
}
