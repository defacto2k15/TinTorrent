//
// Created by defacto on 21.12.16.
//

#include "FileInfo.h"


Resource FileInfo::getResource() {
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

std::ostream &operator<<(std::ostream &os, const FileInfo &info) {
	os << "resource: " << info.resource << " fileSegmentsInfo: " << info.fileSegmentsInfo;
	return os;
}

