//
// Created by defacto on 26.12.16.
//

#include "UpdateInfo.h"

UpdateInfo::UpdateInfo(const std::vector<FileInfo> &newFiles, const std::vector<FileInfo> &deletedFiles) : newFiles(
		newFiles), deletedFiles(deletedFiles) {

}

std::vector<FileInfo> UpdateInfo::getNewFiles() {
	return newFiles;
}

std::vector<FileInfo> UpdateInfo::getDeletedFiles() {
	return deletedFiles;
}

std::ostream &operator<<(std::ostream &os, const UpdateInfo &info) {
	os << "newFiles: " << Help::writeVecContents(info.newFiles) << " deletedFiles: " << Help::writeVecContents(info.deletedFiles);
	return os;
}

bool UpdateInfo::empty() {
	return newFiles.empty() && deletedFiles.empty();
}
