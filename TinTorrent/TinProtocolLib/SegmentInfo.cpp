//
// Created by defacto on 08.12.16.
//

#include "SegmentInfo.h"

SegmentInfo::SegmentInfo( uint16_t segmentIndex, uint16_t payloadLength) : segmentIndex(segmentIndex),
                                                                           payloadLength(payloadLength) {}

uint16_t SegmentInfo::getSegmentIndex() const {
    return segmentIndex;
}

uint16_t SegmentInfo::getPayloadLength() const {
    return payloadLength;
}

bool SegmentInfo::operator==(const SegmentInfo &rhs) const {
	return segmentIndex == rhs.segmentIndex &&
	       payloadLength == rhs.payloadLength;
}

bool SegmentInfo::operator!=(const SegmentInfo &rhs) const {
	return !(rhs == *this);
}
