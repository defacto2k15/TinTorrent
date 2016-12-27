//
// Created by defacto on 22.12.16.
//

#ifndef TINTORRENT_SEGMENTUTILS_H
#define TINTORRENT_SEGMENTUTILS_H


#include <cstddef>
#include <cmath>
#include "Common/Constants.h"

class SegmentUtils {
public:
	static size_t SegmentCount( size_t fileSize ){
		return (size_t)ceil(double(fileSize) / double( Constants::segmentSize));
	}

	static size_t SegmentBytesCount( size_t fileSize ){
		return (size_t)ceil( SegmentCount(fileSize) / 8 );
	}
};


#endif //TINTORRENT_SEGMENTUTILS_H
