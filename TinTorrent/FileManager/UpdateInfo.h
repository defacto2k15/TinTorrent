//
// Created by defacto on 26.12.16.
//

#ifndef TINTORRENT_UPDATEINFO_H
#define TINTORRENT_UPDATEINFO_H


#include <FileManager/ModelEntities/FileInfo.h>

class UpdateInfo {
	std::vector<FileInfo> newFiles;
	std::vector<FileInfo> deletedFiles;
public:
	UpdateInfo(const std::vector<FileInfo> &newFiles, const std::vector<FileInfo> &deletedFiles);

	const std::vector<FileInfo> &getNewFiles() const;

	const std::vector<FileInfo> &getDeletedFiles() const;
};


#endif //TINTORRENT_UPDATEINFO_H
