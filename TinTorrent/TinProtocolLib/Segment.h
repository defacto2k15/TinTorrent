//
// Created by defacto on 13.12.16.
//

#ifndef TINTORRENT_SEGMENT_H
#define TINTORRENT_SEGMENT_H


#include "../Common/Buffer.h"
#include "SegmentInfo.h"

class Segment {
	std::shared_ptr<Buffer> payload;
	SegmentInfo segmentInfo;
public:
	Segment(std::shared_ptr<Buffer> payload, const SegmentInfo &segmentInfo);

	 Buffer &getPayload() ;

	const SegmentInfo getSegmentInfo() const;
};


#endif //TINTORRENT_SEGMENT_H
