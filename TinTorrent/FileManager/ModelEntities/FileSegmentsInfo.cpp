//
// Created by defacto on 21.12.16.
//

#include "FileSegmentsInfo.h"

FileSegmentsInfo::FileSegmentsInfo(const std::vector<bool, std::allocator<bool>> &segmentPresence) : segmentPresence(
		segmentPresence) {
}

bool FileSegmentsInfo::operator==(const FileSegmentsInfo &rhs) const {
	return segmentPresence == rhs.segmentPresence;
}

bool FileSegmentsInfo::operator!=(const FileSegmentsInfo &rhs) const {
	return !(rhs == *this);
}