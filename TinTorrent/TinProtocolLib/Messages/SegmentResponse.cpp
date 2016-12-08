//
// Created by defacto on 08.12.16.
//

#include <cstring>
#include "SegmentResponse.h"

SegmentResponse::SegmentResponse( uint16_t segmentIndex, uint16_t payloadLength, uint8_t *inPayload)
		:  segmentIndex(segmentIndex), payloadLength(payloadLength) {
	constantId[0] = 0xff;
	constantId[1] = 0xff; // todo use configuration
	memcpy(payload, inPayload, payloadLength);
}
