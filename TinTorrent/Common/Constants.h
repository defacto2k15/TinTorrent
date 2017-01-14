//
// Created by defacto on 22.12.16.
//

#ifndef TINTORRENT_CONSTANTS_H
#define TINTORRENT_CONSTANTS_H

#include <string>
#include "json.hpp"

using json = nlohmann::json;

class Constants {
public:
	static std::string metadataFileSuffix;
	static uint16_t  segmentSize;
	static size_t  metadataFileReadingBlockSize;
	static std::string metadataNameRegexp ;
	static std::string resourceNameRegexp;
	static size_t maxBroadcastMessageSize;
	static uint16_t  broadcastPort;
	static std::string broadcastingAddress;
	static uint16_t communicationPort;
	static unsigned maxBadRecievedSegmentsBeforeDisconnect;
	static unsigned secondsBetweenWorkingDirectoryUpdated;
	static unsigned maxSegmentChunkSize;
	static unsigned secondsBetweenBroadcasts;
	static unsigned secondsBetweenStartingDownloads;
	static bool advancedLoggingActive;
	static int serverBacklogSize;
	static bool guiActive;
	static unsigned connectTimeout;
	static bool automaticAnnouncement;
	static bool automaticDownload;

};


#endif //TINTORRENT_CONSTANTS_H
