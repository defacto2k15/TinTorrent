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
	static size_t SegmentCount( size_t fileSize ){
		return (size_t)ceil(double(fileSize) / double( Constants::segmentSize));
	}

	static size_t SegmentBytesCount( size_t fileSize ){
		return (size_t)ceil( SegmentCount(fileSize) / 8 );
	}

	static uint16_t SegmentSize( size_t resourceSize, uint16_t segmentIndex ){
		auto segmentCount = SegmentCount(resourceSize);
		if( segmentIndex < segmentCount - 1 ){
			return Constants::segmentSize;
		} else if ( segmentIndex == segmentCount - 1){
			return  (uint16_t )resourceSize % Constants::segmentSize;
		} else {
			Assertions::fail(Help::Str("Wanted size of segment ",segmentIndex," when segmentSize is ",resourceSize," so there is not that segment"));
			return 0;
		}
	}
};


#endif //TINTORRENT_SEGMENTUTILS_H
