//
// Created by defacto on 27.12.16.
//

#include "SegmentRange.h"

SegmentRange::SegmentRange(unsigned int min, unsigned int max) : min(min), max(max) {}

unsigned int SegmentRange::getMin() const {
	return min;
}

unsigned int SegmentRange::getMax() const {
	return max;
}

std::ostream &operator<<(std::ostream &os, const SegmentRange &range) {
	os << "min: " << range.min << " max: " << range.max;
	return os;
}

bool SegmentRange::operator==(const SegmentRange &rhs) const {
	return min == rhs.min &&
	       max == rhs.max;
}

bool SegmentRange::operator!=(const SegmentRange &rhs) const {
	return !(rhs == *this);
}
