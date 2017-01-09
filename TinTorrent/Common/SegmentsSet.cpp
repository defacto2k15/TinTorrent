//
// Created by defacto on 27.12.16.
//

#include "SegmentsSet.h"

SegmentsSet::SegmentsSet(const SegmentRange &range, const std::vector<Segment> &segments) : range(range), segments(segments) {}

SegmentRange SegmentsSet::getRange() {
	return range;
}

Segment SegmentsSet::getSegment(unsigned segmentIndex) {
	assertIndexInRange(segmentIndex);
	return segments[ segmentIndex - range.getMin()];
}

std::ostream &operator<<(std::ostream &os, const SegmentsSet &set) {
	os << "range: " << set.range << " segments: " << Help::writeVecContents(set.segments);
	return os;
}

void SegmentsSet::assertIndexInRange(unsigned segmentIndex) {
	Assertions::check(range.inRange(segmentIndex), Help::Str("There is no segment of index ", segmentIndex," in range", range));
}
