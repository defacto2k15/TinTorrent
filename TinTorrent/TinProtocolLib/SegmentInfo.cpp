//
// Created by defacto on 08.12.16.
//

#include "SegmentInfo.h"

SegmentInfo::SegmentInfo(char *Payload, uint16_t segmentIndex, uint16_t payloadLength) : Payload(Payload),
                                                                                         segmentIndex(segmentIndex),
                                                                                         payloadLength(payloadLength) {}

char *SegmentInfo::getPayload() const {
    return Payload;
}

uint16_t SegmentInfo::getSegmentIndex() const {
    return segmentIndex;
}

uint16_t SegmentInfo::getPayloadLength() const {
    return payloadLength;
}
