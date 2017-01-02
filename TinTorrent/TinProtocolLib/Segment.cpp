//
// Created by defacto on 13.12.16.
//

#include "Segment.h"

Segment::Segment(const std::shared_ptr<Buffer> payload, const SegmentInfo &segmentInfo) : payload(payload), segmentInfo(segmentInfo) {}

 Buffer &Segment::getPayload()  {
	return *payload;
}

const SegmentInfo Segment::getSegmentInfo() const {
	return segmentInfo;
}

std::ostream &operator<<(std::ostream &os, const Segment &segment) {
	os << "segmentInfo: " << segment.segmentInfo;
	return os;
}
