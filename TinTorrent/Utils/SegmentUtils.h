//
// Created by defacto on 22.12.16.
//

#ifndef TINTORRENT_SEGMENTUTILS_H
#define TINTORRENT_SEGMENTUTILS_H


#include <cstddef>
#include <cmath>
#include <Assertions/Assertions.h>
#include "Common/Constants.h"
#include "MyString.h"

class SegmentUtils {
public:
	static size_t SegmentCount( size_t fileSize );

	static size_t SegmentBytesCount( size_t fileSize );

	static uint16_t SegmentSize( size_t resourceSize, uint16_t segmentIndex );
};


#endif //TINTORRENT_SEGMENTUTILS_H
