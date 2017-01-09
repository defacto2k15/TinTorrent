//
// Created by defacto on 09.01.17.
//

#ifndef TINTORRENT_OUTRESOURCESBEINGDOWNLOADED_H
#define TINTORRENT_OUTRESOURCESBEINGDOWNLOADED_H


#include <ModelEntities/Resource.h>
#include <ModelEntities/TinAddress.h>
#include <Common/SegmentRange.h>

class OutResourcesBeingDownloaded {
public:
	Resource resource;
	std::map<TinAddress, SegmentRange, TinAddressCompare> sourseServersOfDownload;

	OutResourcesBeingDownloaded(const Resource &resource,
	                            const std::map<TinAddress, SegmentRange, TinAddressCompare> &sourseServersOfDownload);
};


#endif //TINTORRENT_OUTRESOURCESBEINGDOWNLOADED_H
