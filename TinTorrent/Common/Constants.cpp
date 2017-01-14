//
// Created by defacto on 22.12.16.
//

#include "Constants.h"

std::string Constants::metadataFileSuffix = ".metadata";
uint16_t  Constants::segmentSize = 4096;
size_t  Constants::metadataFileReadingBlockSize = 4000;
std::string Constants::metadataNameRegexp  = R"foo((.*)\.[[:digit:]]+(\.metadata)$)foo";
std::string Constants::resourceNameRegexp = R"foo((.*)\.([[:digit:]]+)$)foo";
size_t Constants::maxBroadcastMessageSize = 4096;
uint16_t Constants::broadcastPort = 9999;
std::string Constants::broadcastingAddress = "127.0.0.1";
uint16_t Constants::communicationPort = 9998;
unsigned Constants::maxBadRecievedSegmentsBeforeDisconnect = 3;
unsigned Constants::secondsBetweenWorkingDirectoryUpdated = 30;
unsigned Constants::maxSegmentChunkSize = 2;
unsigned Constants::secondsBetweenBroadcasts = 60;
bool Constants::advancedLoggingActive = false;
int Constants::serverBacklogSize = 10;
unsigned Constants::secondsBetweenStartingDownloads = 10;
bool Constants::guiActive = true;
unsigned Constants::connectTimeout = 8;
bool Constants::automaticAnnouncement = true;
bool Constants::automaticDownload = true;