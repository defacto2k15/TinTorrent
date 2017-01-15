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

std::ostream &operator<<(std::ostream &os, const OutLocalResource &resource) {
	os << Help::Str("resource: ", resource.resource.toJson().dump(), " percentDownloaded: ", resource.percentDownloaded,
	    " serversBeingDownloadedInfo: ", resource.serversBeingDownloadedInfo);
	return os;
}
