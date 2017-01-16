//
// Created by defacto on 08.12.16.
//

#include <cstring>
#include "SegmentResponse.h"

size_t SegmentResponse::prefixSize = sizeof(constantId) + sizeof(payloadLength)+sizeof(segmentIndex);