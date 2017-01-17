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
	ConfigLoader( );

	void loadConfigs( std::string configFileName );
private:
	bool parseConfig( std::string filePayload);

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
