//
// Created by defacto on 07.01.17.
//

#include <ProgramInfoProvider/outStructures/OutClientConnectionInfo.h>
#include "ProgramInfoProvider.h"
#include "Kernel/Kernel.h"

ProgramInfoProvider::ProgramInfoProvider(Kernel &kernel) : kernel(kernel), log("ProgramInfoProvider"){

}

std::vector<OutLocalResource> ProgramInfoProvider::getLocalResources() {
	std::promise< std::vector<OutLocalResource>> elementPromise;
	kernel.add( [&]( Kernel &k) {
		auto outValue =  k.getWorkingDirectoryState().getOutLocalResource();
		k.log.debug("InfoProvider getOutLocalResource with ",outValue);
		elementPromise.set_value(outValue);
	});
	return (elementPromise.get_future().get());
}

std::vector<OutTinResourcesInOtherClients> ProgramInfoProvider::getResourcesInOtherClients(){
	std::promise< std::vector<OutTinResourcesInOtherClients>> elementPromise;
	kernel.add( [&]( Kernel &k) {
		auto outValue =   k.getTinNetworkState().getResourcesInOtherClients();
		k.log.debug("InfoProvider getResourcesInOtherClients ", outValue);
		elementPromise.set_value( outValue);
	});
	return (elementPromise.get_future().get());
}

std::vector<Resource> ProgramInfoProvider::getResourcesToDownload(){
	std::promise< std::vector<Resource>> elementPromise;
	kernel.add( [&]( Kernel &k) {
		auto outValue =  k.getResourcesToDownload();
		k.log.debug("InfoProvider getResourcesToDownload ",outValue);
		elementPromise.set_value( outValue);
	});
	return (elementPromise.get_future().get());
 }

std::vector<Resource> ProgramInfoProvider::getRevertedResources(){
	std::promise< std::vector<Resource>> elementPromise;
	kernel.add( [&]( Kernel &k) {
		auto outValue =  k.getRevertedResources();
		k.log.debug("InfoProvider getRevertedResources ",outValue);
		elementPromise.set_value( outValue);
	});
	return (elementPromise.get_future().get());
}

std::vector<OutClientConnectionInfo> ProgramInfoProvider::getConnectionsToOtherServersInfo(){
	std::promise< std::vector<OutClientConnectionInfo>> elementPromise;
	kernel.add( [&]( Kernel &k) {
		auto outValue =  k.getClientThreads().getConnectionsInfo();
		k.log.debug("InfoProvider getConnectionsToOtherServersInfo ", outValue);
		elementPromise.set_value( outValue);
	});
	return (elementPromise.get_future().get());
};

std::vector<OutServerConnectionInfo> ProgramInfoProvider::getConnectionsToUsInfo(){
	std::promise< std::vector<OutServerConnectionInfo>> elementPromise;
	kernel.add( [&]( Kernel &k) {
		auto outValue =  k.getServerThreads().getConnectionsInfo();
		k.log.debug("InfoProvider getConenctionsToUsInfo ", outValue);
		elementPromise.set_value(outValue);
	});
	return (elementPromise.get_future().get());
};

void ProgramInfoProvider::changeResourceBlockState(Resource resource) {
	BlockingElement element;
	kernel.add( [&]( Kernel &k){
		k.log.debug("InfoProvider changeResourceBlockState ",resource);
		k.removeRevertedResource(resource);
		element.unlock();
	});
	element.waitForUnlock();
}

void ProgramInfoProvider::changeResourceAnnouncementState(Resource &resource) {
	BlockingElement element;
	kernel.add( [&]( Kernel &k){
		k.log.debug("InfoProvider changeAnnouncementState ", resource);
		k.getLocalResourcesStateInfo().changeResourceAnnouncementState(resource);
		element.unlock();
	});
	element.waitForUnlock();
}

std::vector<Resource> ProgramInfoProvider::getResourcesThatCanBeAnnounced() {
	std::promise< std::vector<Resource>> elementPromise;
	kernel.add( [&]( Kernel &k) { 
		auto outValue =  k.getLocalResourcesStateInfo().getBroadcastableResources();
		k.log.debug("InfoProvider getResourcesThatCanBeAnnounced ", outValue);
		elementPromise.set_value(outValue);
	});
	return (elementPromise.get_future().get());
}

void ProgramInfoProvider::orderResourceDownload(Resource resource) {
	BlockingElement element;
	kernel.add( [&]( Kernel &k){
		k.log.debug("InfoProvider orderResourceDownload ",resource);
		k.getLocalResourcesStateInfo().changeResourceDownloadabilityState(resource);
		element.unlock();
	});
	element.waitForUnlock();
}
