//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_SEGMENTRESPONSE_H
#define TINTORRENT_SEGMENTRESPONSE_H


#include <cstdint>
#include "Message.h"

struct SegmentResponse : public Message{
public:
	uint8_t constantId[2];
	uint16_t segmentIndex;
	uint16_t payloadLength;
	uint8_t payload[4096];

	SegmentResponse( uint16_t segmentIndex, uint16_t payloadLength, uint8_t *inPayload);
};

#endif //TINTORRENT_SEGMENTRESPONSE_H
