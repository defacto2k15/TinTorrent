//
// Created by defacto on 07.01.17.
//

#ifndef TINTORRENT_OUTLOCALRESOURCE_H
#define TINTORRENT_OUTLOCALRESOURCE_H


#include <ModelEntities/Resource.h>
#include <ModelEntities/TinAddress.h>
#include <Common/SegmentRange.h>
#include <ostream>

class OutLocalResource {
public:
	Resource resource;
	uint8_t percentDownloaded;
	std::map<TinAddress, SegmentRange, TinAddressCompare> serversBeingDownloadedInfo;

	OutLocalResource(const Resource &resource, uint8_t percentDownloaded,
	                 const std::map<TinAddress, SegmentRange, TinAddressCompare> &serversBeingDownloadedInfo);

	OutLocalResource(const Resource &resource, uint8_t percentDownloaded);

	json toJson(){
		json j;
		j["percentDownloaded"] = (int)percentDownloaded;
		j["resource"] = resource.toJson();
		return j;
	}

	friend std::ostream &operator<<(std::ostream &os, const OutLocalResource &resource);
};


#endif //TINTORRENT_OUTLOCALRESOURCE_H
