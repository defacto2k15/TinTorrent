//
// Created by defacto on 09.01.17.
//

#include "OutResourcesBeingDownloaded.h"

OutResourcesBeingDownloaded::OutResourcesBeingDownloaded(const Resource &resource,
                                                         const std::map<TinAddress, SegmentRange, TinAddressCompare> &sourseServersOfDownload)
		: resource(resource), sourseServersOfDownload(sourseServersOfDownload) {}
