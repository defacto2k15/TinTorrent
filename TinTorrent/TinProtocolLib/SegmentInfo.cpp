//
// Created by defacto on 08.12.16.
//

#include "SegmentInfo.h"

SegmentInfo::SegmentInfo(uint8_t *Payload, uint16_t segmentIndex, uint16_t payloadLength) : Payload(Payload),
                                                                                         segmentIndex(segmentIndex),
                                                                                         payloadLength(payloadLength) {}

uint8_t *SegmentInfo::getPayload() const {
    return Payload;
}

uint16_t SegmentInfo::getSegmentIndex() const {
    return segmentIndex;
}

uint16_t SegmentInfo::getPayloadLength() const {
    return payloadLength;
}
