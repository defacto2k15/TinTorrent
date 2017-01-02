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

	static bool parseConstants( std::string &filePayload){
		try{
			json j(filePayload);

			metadataFileSuffix = getElement<std::string>(j, "metadataFileSuffix", metadataFileSuffix);
			// todo rest of settings
		} catch( std::exception &ex){
			std::cout << " Loading configuration failed: "<<ex.what() << std::endl;
			return false;
		}
	}
private:
	template<typename T>
	static T getElement(json &json, std::string elementName, T defaultValue){
		if( json.count(elementName) == 0){
			std::cout << " In configuration file there is no element set of name" << elementName << " Reverting to default" << defaultValue << std::endl;
			return defaultValue;
		}else{
			try{
				return json[elementName];
			} catch( std::exception e ){
				std::cout << "Failed reading configuration element "<<elementName<<" Revering to default "<<defaultValue << std::endl;
				return defaultValue;
			}

		}


	}
};


#endif //TINTORRENT_CONSTANTS_H
