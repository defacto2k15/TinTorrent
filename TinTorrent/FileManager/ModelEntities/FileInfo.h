//
// Created by defacto on 21.12.16.
//

#ifndef TINTORRENT_FILEINFO_H
#define TINTORRENT_FILEINFO_H


#include "../../ModelEntities/Resource.h"
#include "FileSegmentsInfo.h"

class FileInfo {
	Resource resource;
	FileSegmentsInfo fileSegmentsInfo;
public:
	FileInfo(const Resource &resource, const FileSegmentsInfo &fileSegmentsInfo);

	const Resource getResource() const;

	const FileSegmentsInfo getFileSegmentsInfo() const;

	bool operator==(const FileInfo &rhs) const;

	bool operator!=(const FileInfo &rhs) const;
};


#endif //TINTORRENT_FILEINFO_H
