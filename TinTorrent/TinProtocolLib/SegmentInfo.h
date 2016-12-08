//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_SEGMENTINFO_H
#define TINTORRENT_SEGMENTINFO_H


#include <stdint-gcc.h>

class SegmentInfo {
    char *Payload;
    uint16_t  segmentIndex;
    uint16_t  payloadLength;
public:
    SegmentInfo(char *Payload, uint16_t segmentIndex, uint16_t payloadLength);

    char *getPayload() const;

    uint16_t getSegmentIndex() const;

    uint16_t getPayloadLength() const;
};


#endif //TINTORRENT_SEGMENTINFO_H
