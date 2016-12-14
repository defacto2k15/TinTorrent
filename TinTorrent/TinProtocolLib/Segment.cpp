//
// Created by defacto on 13.12.16.
//

#include "Segment.h"

Segment::Segment(const Buffer &payload, const SegmentInfo &segmentInfo) : payload(payload), segmentInfo(segmentInfo) {}

const Buffer Segment::getPayload() const {
	return payload;
}

const SegmentInfo Segment::getSegmentInfo() const {
	return segmentInfo;
}
