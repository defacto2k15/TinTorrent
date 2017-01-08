//
// Created by defacto on 21.12.16.
//

#ifndef TINTORRENT_FILESEGMENTSINFO_H
#define TINTORRENT_FILESEGMENTSINFO_H


#include <vector>
#include <cstddef>
#include <string>
#include <ios>
#include <fstream>
#include <memory>
#include <Utils/BitUtils.h>
#include <ostream>
#include "Common/Constants.h"

class FileSegmentsInfo {
	std::vector<bool> segmentPresence;
public:
	FileSegmentsInfo(const std::vector<bool, std::allocator<bool>> &segmentPresence);
	FileSegmentsInfo ( size_t segmentCount );
	FileSegmentsInfo();;
	FileSegmentsInfo(size_t segmentCount, std::string metadataFilePath);

	bool operator==(const FileSegmentsInfo &rhs) const;

	bool operator!=(const FileSegmentsInfo &rhs) const;

	bool getSegmentPresence( size_t segmentIndex );

	void setSegmentPresence( size_t segmentIndex, bool newValue );

	friend std::ostream &operator<<(std::ostream &os, const FileSegmentsInfo &info);
};


#endif //TINTORRENT_FILESEGMENTSINFO_H
