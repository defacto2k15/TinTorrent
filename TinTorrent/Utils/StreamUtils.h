//
// Created by defacto on 22.12.16.
//

#ifndef TINTORRENT_STREAMUTILS_H
#define TINTORRENT_STREAMUTILS_H


#include <iosfwd>
#include <vector>
#include <math.h>
#include <cstdint>
#include <memory>
#include "SegmentUtils.h"
#include <ostream>
#include <fstream>
#include <Common/Buffer.h>

class StreamUtils {
public:
	static size_t writeToStream( std::ofstream &ofs, std::vector<bool> payload){
		size_t  segmentCount = payload.size();
		size_t bytesCount = (size_t)ceil(double(segmentCount)/8.f);
		std::unique_ptr<char[]> buffer (new char[bytesCount]);
		for( size_t i = 0; i < bytesCount; i++ ){
			uint8_t byte = 0;
			int bitsToWrite = std::min((int)(segmentCount - i * 8), 8);
			for( int k = 0; k < bitsToWrite; k++){
				byte |= ( payload[i*8 + k] << k);
			}
			buffer.operator[](i) = byte;
		}
		ofs.write(buffer.get(), bytesCount);
		return bytesCount;
	}

	static void writeToStream( std::ofstream &ofs, Buffer &buffer){
		ofs.write((const char*)(buffer.getData()), buffer.getSize());
	}
};


#endif //TINTORRENT_STREAMUTILS_H
