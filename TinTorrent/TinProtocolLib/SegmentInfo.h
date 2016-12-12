//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_SEGMENTINFO_H
#define TINTORRENT_SEGMENTINFO_H


#include <stdint-gcc.h>

class SegmentInfo {
    uint8_t *Payload;
    uint16_t  segmentIndex;
    uint16_t  payloadLength;
public:
    SegmentInfo(uint8_t *Payload, uint16_t segmentIndex, uint16_t payloadLength);

    uint8_t *getPayload() const;

    uint16_t getSegmentIndex() const;

    uint16_t getPayloadLength() const;
};


#endif //TINTORRENT_SEGMENTINFO_H
