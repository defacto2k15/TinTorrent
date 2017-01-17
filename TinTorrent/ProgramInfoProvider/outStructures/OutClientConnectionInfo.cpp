//
// Created by defacto on 08.01.17.
//

#include "OutClientConnectionInfo.h"

OutClientConnectionInfo::OutClientConnectionInfo(TinAddress address) : address(address){

}

OutClientConnectionInfo::OutClientConnectionInfo(TinAddress address, Resource resource) : address(address),
                                                                                          downloadedResource(std::experimental::make_optional(resource)){
}

bool OutClientConnectionInfo::isResourceBeingDownloaded() {
	return (bool)downloadedResource;
}

Resource OutClientConnectionInfo::getResource() {
	return downloadedResource.value();
}

std::ostream &operator<<(std::ostream &os, const OutClientConnectionInfo &info) {
	os << Help::Str("address: ",info.address," downloadedResource: ", info.downloadedResource);
	return os;
}

json OutClientConnectionInfo::toJson() {
	json j;
	j["address"] = address.toString();
	if( downloadedResource) {
		j["downloadedResource"] = downloadedResource->toJson();
	}
	return j;
}
