//
// Created by defacto on 22.12.16.
//

#include "Constants.h"

std::string Constants::metadataFileSuffix = ".metadata";
uint16_t  Constants::segmentSize = 4096;
size_t  Constants::metadataFileReadingBlockSize = 4000;
std::string Constants::metadataNameRegexp  = R"foo((.*)\.[[:digit:]]+(\.metadata)$)foo";
std::string Constants::resourceNameRegexp = R"foo((.*)\.([[:digit:]]+)$)foo";
