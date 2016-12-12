//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_SEGMENTRESPONSE_H
#define TINTORRENT_SEGMENTRESPONSE_H


#include <cstdint>
#include "Message.h"
#include "../SegmentInfo.h"

struct SegmentResponse : public Message{
public:
	uint8_t constantId[2];
	uint16_t segmentIndex;
	uint16_t payloadLength;
	uint8_t *payload;

	SegmentResponse(SegmentInfo info) : segmentIndex(info.getSegmentIndex()),
	                                                      payloadLength(info.getPayloadLength()),
                                                          payload(info.getPayload()){
		constantId[0] = 0xff;
		constantId[1] = 0xff;
	}

	SegmentResponse(Buffer &buffer){
		//todo
	}

	SegmentInfo getSegmentInfo() {
		return SegmentInfo(payload, segmentIndex, payloadLength);
	}

	void serializeTo(Buffer &buffer) override {
		//todo
	}
};

#endif //TINTORRENT_SEGMENTRESPONSE_H
