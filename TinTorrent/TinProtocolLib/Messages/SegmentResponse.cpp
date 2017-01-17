//
// Created by defacto on 08.12.16.
//

#include <cstring>
#include "SegmentResponse.h"

size_t SegmentResponse::prefixSize = sizeof(constantId) + sizeof(payloadLength)+sizeof(segmentIndex);

SegmentResponse::SegmentResponse(uint16_t segmentIndex, uint16_t payloadLength, uint8_t *inPayload) :
		payloadLength(payloadLength),  segmentIndex(segmentIndex){
	Assertions::check(payloadLength < 25000-16, "PayloadLength too big");
	constantId[0] = 0xff; //todo take from configuration
	constantId[1] = 0xff;
	memcpy(payload, inPayload, payloadLength);
}

void SegmentResponse::validateFields() {
	Assertions::check(constantId[0] == 0xff, "ConstantId first byte");
	Assertions::check(constantId[1] == 0xff, "ConstantId second byte");
	Assertions::check(payloadLength < 25000-16, "PayloadLength too big");
}

SegmentInfo SegmentResponse::getSegmentInfo() {
	return SegmentInfo(segmentIndex, payloadLength);
}

void SegmentResponse::writeSegmentResponseToBuffer(SegmentInfo &segmentInfo, uint8_t *payload, Buffer &buffer) {
	uint8_t * bufferPtr = buffer.getData();
	*((uint8_t *)bufferPtr) = 0xff; //todo
	*((uint8_t *)bufferPtr+1) = 0xff; //todo
	*((uint16_t *)(bufferPtr+2)) = segmentInfo.getPayloadLength();
	*((uint16_t *)(bufferPtr+4)) = segmentInfo.getSegmentIndex();
	buffer.setData(6, payload, segmentInfo.getPayloadLength());
}

size_t SegmentResponse::getWholeResponseSize() {
	return sizeof(constantId)+sizeof(segmentIndex)+sizeof(payloadLength) + payloadLength;
}
