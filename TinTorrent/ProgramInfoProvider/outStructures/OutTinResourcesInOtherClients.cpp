//
// Created by defacto on 08.01.17.
//

#include "OutTinResourcesInOtherClients.h"

OutTinResourcesInOtherClients::OutTinResourcesInOtherClients(const TinAddress &address,
                                                             const std::vector<Resource> &resources) : address(address),
                                                                                                       resources(
		                                                                                                       resources) {}

 TinAddress OutTinResourcesInOtherClients::getAddress()  {
	return address;
}

 std::vector<Resource> OutTinResourcesInOtherClients::getResources()  {
	return resources;
}

std::ostream &operator<<(std::ostream &os, const OutTinResourcesInOtherClients &clients) {
	os << Help::Str("OutTinResourcesInOtherCities: address: ",clients.address, " resources: ", clients.resources);
	return os;
}

json OutTinResourcesInOtherClients::toJson() {
	json j;
	j["address"] = address.toString();
	std::vector<json> resourcesVec;
	for( auto &res : resources){
		resourcesVec.push_back(res.toJson());
	}
	j["resources"] = resourcesVec;
	return j;
}
