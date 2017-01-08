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
	static size_t writeToStream( std::ofstream &ofs, std::vector<bool> payload);

	static void writeToStream( std::ofstream &ofs, Buffer &buffer);
};


#endif //TINTORRENT_STREAMUTILS_H
