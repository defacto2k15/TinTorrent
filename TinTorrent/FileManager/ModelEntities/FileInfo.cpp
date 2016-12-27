//
// Created by defacto on 21.12.16.
//

#include "FileInfo.h"


const Resource FileInfo::getResource() const {
	return resource;
}

FileSegmentsInfo &FileInfo::getFileSegmentsInfo() {
	return fileSegmentsInfo;
}

FileInfo::FileInfo(const Resource &resource, const FileSegmentsInfo &fileSegmentsInfo) : resource(resource),
                                                                                         fileSegmentsInfo(
		                                                                                         fileSegmentsInfo) {
}

bool FileInfo::operator==(const FileInfo &rhs) const {
	return resource == rhs.resource &&
	       fileSegmentsInfo == rhs.fileSegmentsInfo;
}

bool FileInfo::operator!=(const FileInfo &rhs) const {
	return !(rhs == *this);
}

