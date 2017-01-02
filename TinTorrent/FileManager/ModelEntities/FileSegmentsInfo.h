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
	FileSegmentsInfo ( size_t segmentCount ) : segmentPresence(segmentCount, false){ }
	FileSegmentsInfo(){};
	FileSegmentsInfo(size_t segmentCount, std::string metadataFilePath){
		std::ifstream source(metadataFilePath.c_str(), std::ios_base::binary);

		size_t  bufferSize = Constants::metadataFileReadingBlockSize;
		std::unique_ptr<char[]> buffer( new char(bufferSize ));
		segmentPresence.resize(segmentCount, false);
		ssize_t  readBytes = 0;
		int i = 0;
		do{
			source.read(buffer.get(), bufferSize);
			readBytes = source.gcount();

			for(int k = 0; k < readBytes; k++ ){
				size_t firstProcessedBitCount = i * bufferSize + k*8;
				int bitsToProcess = std::min(8, int(segmentCount - firstProcessedBitCount));
				for( int l = 0; l < bitsToProcess; l++){
					segmentPresence[firstProcessedBitCount + l]
							= BitUtils::getNthBit((uint8_t) *(buffer.get() + i*bufferSize + k ), l);
				}
			}

			i++;
		}
		while( source);
	}

	bool operator==(const FileSegmentsInfo &rhs) const;

	bool operator!=(const FileSegmentsInfo &rhs) const;

	bool getSegmentPresence( size_t segmentIndex ){
		return segmentPresence[segmentIndex];
	}

	void setSegmentPresence( size_t segmentIndex, bool newValue ){
		segmentPresence[segmentIndex] = newValue;
	}

	friend std::ostream &operator<<(std::ostream &os, const FileSegmentsInfo &info);
};


#endif //TINTORRENT_FILESEGMENTSINFO_H
