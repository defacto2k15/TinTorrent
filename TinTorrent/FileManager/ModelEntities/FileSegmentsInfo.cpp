//
// Created by defacto on 21.12.16.
//

#include <Utils/MyString.h>
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

std::ostream &operator<<(std::ostream &os, const FileSegmentsInfo &info) {
	os << "segmentPresence: " << Help::writeVecContents(info.segmentPresence);
	return os;
}

FileSegmentsInfo::FileSegmentsInfo(size_t segmentCount) : segmentPresence(segmentCount, false){ }

FileSegmentsInfo::FileSegmentsInfo() {}

FileSegmentsInfo::FileSegmentsInfo(size_t segmentCount, std::string metadataFilePath) {
	std::ifstream source(metadataFilePath.c_str(), std::ios_base::binary);

	size_t  bufferSize = Constants::metadataFileReadingBlockSize;
	std::unique_ptr<char[]> buffer( new char[bufferSize ]);
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

bool FileSegmentsInfo::getSegmentPresence(size_t segmentIndex) {
	return segmentPresence[segmentIndex];
}

void FileSegmentsInfo::setSegmentPresence(size_t segmentIndex, bool newValue) {
	segmentPresence[segmentIndex] = newValue;
}
