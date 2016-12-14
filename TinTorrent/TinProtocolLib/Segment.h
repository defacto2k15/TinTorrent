//
// Created by defacto on 13.12.16.
//

#ifndef TINTORRENT_SEGMENT_H
#define TINTORRENT_SEGMENT_H


#include "../Common/Buffer.h"
#include "SegmentInfo.h"

class Segment {
	Buffer payload;
	SegmentInfo segmentInfo;
public:
	Segment(const Buffer &payload, const SegmentInfo &segmentInfo);

	const Buffer getPayload() const;

	const SegmentInfo getSegmentInfo() const;
};


#endif //TINTORRENT_SEGMENT_H
