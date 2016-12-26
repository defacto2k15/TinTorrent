//
// Created by defacto on 26.12.16.
//

#include "UpdateInfo.h"

UpdateInfo::UpdateInfo(const std::vector<FileInfo> &newFiles, const std::vector<FileInfo> &deletedFiles) : newFiles(
		newFiles), deletedFiles(deletedFiles) {

}

const std::vector<FileInfo> &UpdateInfo::getNewFiles() const {
	return newFiles;
}

const std::vector<FileInfo> &UpdateInfo::getDeletedFiles() const {
	return deletedFiles;
}
