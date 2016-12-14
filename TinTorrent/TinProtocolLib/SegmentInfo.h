//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_SEGMENTINFO_H
#define TINTORRENT_SEGMENTINFO_H

#include <stdint-gcc.h>
#include <json.hpp>

using json = nlohmann::json;

class SegmentInfo {
    uint16_t  segmentIndex;
    uint16_t  payloadLength;
public:
    SegmentInfo( uint16_t segmentIndex, uint16_t payloadLength);

	SegmentInfo(json j) :
			  segmentIndex( j["SegmentIndex"]),
			  payloadLength(j["PayloadLength"]){
	}

    uint16_t getSegmentIndex() const;

    uint16_t getPayloadLength() const;

	json toJson(){
		return json{
				{"SegmentIndex", segmentIndex},
				{"PayloadLength", payloadLength}
		};
	}

	bool operator==(const SegmentInfo &rhs) const;

	bool operator!=(const SegmentInfo &rhs) const;
};


#endif //TINTORRENT_SEGMENTINFO_H
