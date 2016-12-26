//
// Created by defacto on 22.12.16.
//

#ifndef TINTORRENT_CONSTANTS_H
#define TINTORRENT_CONSTANTS_H

#include <string>

class Constants {
public:
	static std::string metadataFileSuffix;
	static uint16_t  segmentSize;
	static size_t  metadataFileReadingBlockSize;
	static std::string metadataNameRegexp ;
	static std::string resourceNameRegexp;
};


#endif //TINTORRENT_CONSTANTS_H
