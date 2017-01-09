//
// Created by defacto on 07.01.17.
//

#include <ProgramInfoProvider/outStructures/OutClientConnectionInfo.h>
#include "ProgramInfoProvider.h"
#include "Kernel/Kernel.h"

ProgramInfoProvider::ProgramInfoProvider(Kernel &kernel) : kernel(kernel){}

std::vector<OutLocalResource> ProgramInfoProvider::getLocalResources() {
	BlockingElement blockingElement;
	std::promise< std::vector<OutLocalResource>> elementPromise;
	kernel.add( [&]( Kernel &k) { elementPromise.set_value( k.getWorkingDirectoryState().getOutLocalResource()); });
	return (elementPromise.get_future().get());
}

std::vector<OutTinResourcesInOtherClients> ProgramInfoProvider::getResourcesInOtherClients(){
	BlockingElement blockingElement;
	std::promise< std::vector<OutTinResourcesInOtherClients>> elementPromise;
	kernel.add( [&]( Kernel &k) { elementPromise.set_value( k.getTinNetworkState().getResourcesInOtherClients()); });
	return (elementPromise.get_future().get());
}

std::vector<Resource> ProgramInfoProvider::getResourcesToDownload(){
	BlockingElement blockingElement;
	std::promise< std::vector<Resource>> elementPromise;
	kernel.add( [&]( Kernel &k) { elementPromise.set_value( k.getResourcesToDownload()); });
	return (elementPromise.get_future().get());
 }

std::vector<Resource> ProgramInfoProvider::getRevertedResources(){
	BlockingElement blockingElement;
	std::promise< std::vector<Resource>> elementPromise;
	kernel.add( [&]( Kernel &k) { elementPromise.set_value( k.getRevertedResources()); });
	return (elementPromise.get_future().get());
}

std::vector<OutClientConnectionInfo> ProgramInfoProvider::getConnectionsToOtherServersInfo(){
	BlockingElement blockingElement;
	std::promise< std::vector<OutClientConnectionInfo>> elementPromise;
	kernel.add( [&]( Kernel &k) { elementPromise.set_value( k.getClientThreads().getConnectionsInfo()); });
	return (elementPromise.get_future().get());
};

std::vector<OutServerConnectionInfo> ProgramInfoProvider::getConnectionsToUsInfo(){
	BlockingElement blockingElement;
	std::promise< std::vector<OutServerConnectionInfo>> elementPromise;
	kernel.add( [&]( Kernel &k) { elementPromise.set_value( k.getServerThreads().getConnectionsInfo()); });
	return (elementPromise.get_future().get());
};

void ProgramInfoProvider::changeResourceBlockState(Resource resource) {
	//TODO change block state of resource
}