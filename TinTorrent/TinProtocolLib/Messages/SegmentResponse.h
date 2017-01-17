//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_SEGMENTRESPONSE_H
#define TINTORRENT_SEGMENTRESPONSE_H


#include <cstdint>
#include "Message.h"
#include "../SegmentInfo.h"

struct SegmentResponse {
public:
	uint8_t constantId[2];
	uint16_t payloadLength;
	uint16_t segmentIndex;
	uint8_t payload[25000]; // todo take length from constants XXX TODO

	SegmentResponse(uint16_t segmentIndex, uint16_t payloadLength, uint8_t *inPayload);

	void validateFields();

	SegmentInfo getSegmentInfo();

	static void writeSegmentResponseToBuffer( SegmentInfo &segmentInfo, uint8_t *payload, Buffer &buffer);

	size_t getWholeResponseSize();

	static size_t prefixSize;

};

#endif //TINTORRENT_SEGMENTRESPONSE_H
