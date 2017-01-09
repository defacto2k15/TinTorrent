//
// Created by defacto on 07.01.17.
//

#include "OutLocalResource.h"

OutLocalResource::OutLocalResource(const Resource &resource, uint8_t percentDownloaded) : resource(resource),
                                                                                           percentDownloaded(
		                                                                                           percentDownloaded) {}

OutLocalResource::OutLocalResource(const Resource &resource, uint8_t percentDownloaded,
                                   const std::map<TinAddress, SegmentRange, TinAddressCompare> &serversBeingDownloadedInfo)
		: resource(resource), percentDownloaded(percentDownloaded),
		  serversBeingDownloadedInfo(serversBeingDownloadedInfo) {}
