//
// Created by defacto on 08.01.17.
//

#ifndef TINTORRENT_OUTCLIENTCONNECTIONINFO_H
#define TINTORRENT_OUTCLIENTCONNECTIONINFO_H


#include <ModelEntities/TinAddress.h>
#include <experimental/optional>
#include <ModelEntities/Resource.h>

class OutClientConnectionInfo {
	TinAddress address;
	std::experimental::optional<Resource> downloadedResource;
public:
	OutClientConnectionInfo( TinAddress address );

	OutClientConnectionInfo( TinAddress address, Resource resource );

	bool isResourceBeingDownloaded();

	Resource getResource();

	json toJson(){
		json j;
		j["address"] = address.toString();
		if( downloadedResource) {
			j["downloadedResource"] = downloadedResource->toJson();
		}
		return j;
	}
};


#endif //TINTORRENT_OUTCLIENTCONNECTIONINFO_H
