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
