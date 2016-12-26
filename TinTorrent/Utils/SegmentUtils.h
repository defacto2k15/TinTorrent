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
		return ceil(double(fileSize) / double( Constants::segmentSize));
	}
};


#endif //TINTORRENT_SEGMENTUTILS_H
