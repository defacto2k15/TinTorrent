//
// Created by defacto on 27.12.16.
//

#ifndef TINTORRENT_SEGMENTSSET_H
#define TINTORRENT_SEGMENTSSET_H


#include <TinProtocolLib/Segment.h>
#include <Utils/MyString.h>
#include <ostream>
#include "SegmentRange.h"

class SegmentsSet {
	SegmentRange range;
	std::vector<Segment> segments;
public:
	SegmentsSet(const SegmentRange &range, const std::vector<Segment> &segments) : range(range), segments(segments) {}

	SegmentRange  getRange(){
		return range;
	}

	Segment getSegment(unsigned segmentIndex){
		assertIndexInRange(segmentIndex);
		return segments[ segmentIndex - range.getMin()];
	}

	friend std::ostream &operator<<(std::ostream &os, const SegmentsSet &set) {
		os << "range: " << set.range << " segments: " << Help::writeVecContents(set.segments).str();
		return os;
	}

private:
	void assertIndexInRange( unsigned segmentIndex){
		Assertions::check(range.inRange(segmentIndex), Help::Str("There is no segment of index ", segmentIndex," in range", range));
	}
};


#endif //TINTORRENT_SEGMENTSSET_H
