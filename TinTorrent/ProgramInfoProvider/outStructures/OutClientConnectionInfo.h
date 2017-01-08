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
	OutClientConnectionInfo( TinAddress address ): address(address){

	}

	OutClientConnectionInfo( TinAddress address, Resource resource ) : address(address),
	                                                                     downloadedResource(std::experimental::make_optional(resource)){
	}

	bool isResourceBeingDownloaded(){
		return (bool)downloadedResource;
	}

	Resource getResource(){
		return downloadedResource.value();
	}
};


#endif //TINTORRENT_OUTCLIENTCONNECTIONINFO_H
