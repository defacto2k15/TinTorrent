//
// Created by defacto on 10.01.17.
//

#ifndef TINTORRENT_CONFIGLOADER_H
#define TINTORRENT_CONFIGLOADER_H


#include <Logging/LogBase.h>
#include <fstream>

class ConfigLoader {
	LogBase log;
public:
	ConfigLoader( ) : log("ConfigLoader"){
	}

	void loadConfigs( std::string configFileName ) {
		if (configFileName.empty()) {
			log.debug("Configuration filepath is empty - wont load file ");
			return;
		}
		std::ifstream t(configFileName);
		if (!t) {
			log.debug("Cannot read configuration file ", configFileName, " reverting to default");
			return;
		}
		std::string filePayload((std::istreambuf_iterator<char>(t)),
		                        std::istreambuf_iterator<char>());

		if (parseConfig(filePayload)) {
			log.debug("Loading configuration file ", configFileName, " OK");
		} else {
			log.debug("Failed processing configuration file ", configFileName, " reverting to default");
		}
	}
private:
	bool parseConfig( std::string filePayload){
		try{
			auto j = json::parse(filePayload);

			Constants::metadataFileSuffix = getElement<std::string>(j, "metadataFileSuffix", Constants::metadataFileSuffix);
			Constants::segmentSize = getElement<uint16_t  >(j, "segmentSize", Constants::segmentSize);
			Constants::metadataFileReadingBlockSize = getElement<size_t  >(j, "metadataFileReadingBlockSize", Constants::metadataFileReadingBlockSize);
			Constants::metadataNameRegexp  = getElement<std::string >(j, "metadataNameRegexp ", Constants::metadataNameRegexp );
			Constants::resourceNameRegexp = getElement<std::string >(j, "resourceNameRegexp", Constants::resourceNameRegexp);
			Constants::maxBroadcastMessageSize = getElement<size_t >(j, "maxBroadcastMessageSize", Constants::maxBroadcastMessageSize);
			Constants::broadcastPort = getElement<uint16_t  >(j, "broadcastPort", Constants::broadcastPort);
			Constants::broadcastingAddress = getElement<std::string >(j, "broadcastingAddress", Constants::broadcastingAddress);
			Constants::communicationPort = getElement<uint16_t >(j, "communicationPort", Constants::communicationPort);
			Constants::maxBadRecievedSegmentsBeforeDisconnect = getElement<unsigned >(j, "maxBadRecievedSegmentsBeforeDisconnect", Constants::maxBadRecievedSegmentsBeforeDisconnect);
			Constants::secondsBetweenWorkingDirectoryUpdated = getElement<unsigned >(j, "secondsBetweenWorkingDirectoryUpdated", Constants::secondsBetweenWorkingDirectoryUpdated);
			Constants::maxSegmentChunkSize = getElement<unsigned >(j, "maxSegmentChunkSize", Constants::maxSegmentChunkSize);
			Constants::secondsBetweenBroadcasts = getElement<unsigned >(j, "secondsBetweenBroadcasts", Constants::secondsBetweenBroadcasts);
			Constants::secondsBetweenStartingDownloads = getElement<unsigned >(j, "secondsBetweenStartingDownloads", Constants::secondsBetweenStartingDownloads);
			Constants::advancedLoggingActive = getElement<bool >(j, "advancedLoggingActive", Constants::advancedLoggingActive);
			Constants::serverBacklogSize = getElement<int >(j, "serverBacklogSize", Constants::serverBacklogSize);
			Constants::guiActive = getElement<bool >(j, "guiActive", Constants::guiActive);
			Constants::connectTimeout = getElement<unsigned >(j, "connectTimeout", Constants::connectTimeout);
			Constants::automaticAnnouncement = getElement<bool >(j, "automaticAnnouncement", Constants::automaticAnnouncement);
			Constants::automaticDownload = getElement<bool >(j, "automaticDownload", Constants::automaticDownload);
		} catch( std::exception &ex){
			std::cerr << " Loading configuration failed: "<<ex.what() << std::endl;
			return false;
		}
		return true;
	}

	template<typename T>
	T getElement(json &json, std::string elementName, T defaultValue){
		if( json.count(elementName) == 0){
			std::cerr << " In configuration file there is no element set of name " << elementName << " Reverting to default " << defaultValue << std::endl;
			return defaultValue;
		}else{
			try{
				T value = json[elementName];
				log.debug("Element ",elementName," has value ",value);
				return value;
			} catch( std::exception e ){
				std::cerr << "Failed reading configuration element "<<elementName<<" Revering to default "<<defaultValue << std::endl;
				return defaultValue;
			}
		}
	}
};


#endif //TINTORRENT_CONFIGLOADER_H
