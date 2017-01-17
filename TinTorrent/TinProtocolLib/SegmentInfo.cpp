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

std::ostream &operator<<(std::ostream &os, const SegmentInfo &info) {
	os << "segmentIndex: " << info.segmentIndex << " payloadLength: " << info.payloadLength;
	return os;
}

SegmentInfo::SegmentInfo(json j) :
		segmentIndex( j["SegmentIndex"]),
		payloadLength(j["PayloadLength"]){
}

json SegmentInfo::toJson() {
	return json{
			{"SegmentIndex", segmentIndex},
			{"PayloadLength", payloadLength}
	};
}
