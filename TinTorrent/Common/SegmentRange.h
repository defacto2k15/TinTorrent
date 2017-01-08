//
// Created by defacto on 27.12.16.
//

#ifndef TINTORRENT_SEGMENTRANGE_H
#define TINTORRENT_SEGMENTRANGE_H


#include <ostream>

class SegmentRange {
	unsigned min;
	unsigned max;
public:
	SegmentRange(unsigned int min, unsigned int max);
	SegmentRange();

	unsigned int getMin() const;

	unsigned int getMax() const;

	friend std::ostream &operator<<(std::ostream &os, const SegmentRange &range);

	bool inRange(unsigned segmentIndex ) const;

	bool empty();

	bool operator==(const SegmentRange &rhs) const;

	bool operator!=(const SegmentRange &rhs) const;


};


#endif //TINTORRENT_SEGMENTRANGE_H
