//
// Created by defacto on 22.12.16.
//

#include "SegmentUtils.h"

size_t SegmentUtils::SegmentCount(size_t fileSize) {
	return (size_t)ceil(double(fileSize) / double( Constants::segmentSize));
}

size_t SegmentUtils::SegmentBytesCount(size_t fileSize) {
	return (size_t)ceil( SegmentCount(fileSize) / 8 );
}

uint16_t SegmentUtils::SegmentSize(size_t resourceSize, uint16_t segmentIndex) {
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
