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
	SegmentsSet(const SegmentRange &range, const std::vector<Segment> &segments);

	SegmentRange  getRange();

	Segment getSegment(unsigned segmentIndex);

	friend std::ostream &operator<<(std::ostream &os, const SegmentsSet &set);

private:
	void assertIndexInRange( unsigned segmentIndex);
};


#endif //TINTORRENT_SEGMENTSSET_H
