//
// Created by defacto on 07.01.17.
//

#include <ProgramInfoProvider/outStructures/OutClientConnectionInfo.h>
#include "ProgramInfoProvider.h"
#include "Kernel/Kernel.h"

ProgramInfoProvider::ProgramInfoProvider(Kernel &kernel) : kernel(kernel), log("ProgramInfoProvider"){

}

std::vector<OutLocalResource> ProgramInfoProvider::getLocalResources() {
	log.debug("getLocalResources");
	std::promise< std::vector<OutLocalResource>> elementPromise;
	kernel.add( [&]( Kernel &k) { elementPromise.set_value( k.getWorkingDirectoryState().getOutLocalResource()); });
	auto outVal = elementPromise.get_future().get();
	log.debug("getLocalResources ", outVal);
	return (outVal);
}

std::vector<OutTinResourcesInOtherClients> ProgramInfoProvider::getResourcesInOtherClients(){
	log.debug("getResourcesInOtherClients");
	std::promise< std::vector<OutTinResourcesInOtherClients>> elementPromise;
	kernel.add( [&]( Kernel &k) { elementPromise.set_value( k.getTinNetworkState().getResourcesInOtherClients()); });
	auto outVal = elementPromise.get_future().get();
	log.debug("getResourcesInOtherClients out ", outVal);
	return (outVal);
}

std::vector<Resource> ProgramInfoProvider::getResourcesToDownload(){
	log.debug("getResourcesToDownload");
	std::promise< std::vector<Resource>> elementPromise;
	kernel.add( [&]( Kernel &k) { elementPromise.set_value( k.getResourcesToDownload()); });
	auto outVal = elementPromise.get_future().get();
	log.debug("getResourcesToDownload out ", outVal);
	return (outVal);
 }

std::vector<Resource> ProgramInfoProvider::getRevertedResources(){
	log.debug("getRevertedResources");
	std::promise< std::vector<Resource>> elementPromise;
	kernel.add( [&]( Kernel &k) { elementPromise.set_value( k.getRevertedResources()); });
	auto outVal = elementPromise.get_future().get();
	log.debug("getLocalResources ", outVal);
	return (outVal);
}

std::vector<OutClientConnectionInfo> ProgramInfoProvider::getConnectionsToOtherServersInfo(){
	log.debug("getConnectionsToOtherServersInfo");
	std::promise< std::vector<OutClientConnectionInfo>> elementPromise;
	kernel.add( [&]( Kernel &k) { elementPromise.set_value( k.getClientThreads().getConnectionsInfo()); });
	auto outVal = elementPromise.get_future().get();
	log.debug("getConnectionsToOtherServersInfo out ", outVal);
	return (outVal);
};

std::vector<OutServerConnectionInfo> ProgramInfoProvider::getConnectionsToUsInfo(){
	log.debug("getConnectionsToUsInfo");
	std::promise< std::vector<OutServerConnectionInfo>> elementPromise;
	kernel.add( [&]( Kernel &k) { elementPromise.set_value( k.getServerThreads().getConnectionsInfo()); });
	auto outVal = elementPromise.get_future().get();
	log.debug("getConnectionsToUsInfo out ", outVal);
	return (outVal);
};

void ProgramInfoProvider::changeResourceBlockState(Resource resource) {
	log.debug(Help::Str("changeResourceBlockState with resource ",resource.toJson().dump()));
	BlockingElement element;
	kernel.add( [&]( Kernel &k){
		k.removeRevertedResource(resource);
		element.unlock();
	});
	element.waitForUnlock();
}

void ProgramInfoProvider::changeResourceAnnouncementState(Resource &resource) {
	log.debug(Help::Str("changeResourceAnnouncementState with resource ",resource.toJson().dump()));
	BlockingElement element;
	kernel.add( [&]( Kernel &k){
		k.getLocalResourcesStateInfo().changeResourceAnnouncementState(resource);
		element.unlock();
	});
	element.waitForUnlock();
}

std::vector<Resource> ProgramInfoProvider::getResourcesThatCanBeAnnounced() {
	log.debug("getResourcesThatCanBeAnnounced");
	std::promise< std::vector<Resource>> elementPromise;
	kernel.add( [&]( Kernel &k) { elementPromise.set_value( k.getLocalResourcesStateInfo().getBroadcastableResources()); });
	auto outVal = elementPromise.get_future().get();
	log.debug("getResourcesThatCanBeAnnounced out ", outVal);
	return (outVal);
}

void ProgramInfoProvider::orderResourceDownload(Resource resource) {
	log.debug(Help::Str("orderResourceDownload with resource ",resource.toJson().dump()));
	BlockingElement element;
	kernel.add( [&]( Kernel &k){
		k.getLocalResourcesStateInfo().changeResourceDownloadabilityState(resource);
		element.unlock();
	});
	element.waitForUnlock();
}
