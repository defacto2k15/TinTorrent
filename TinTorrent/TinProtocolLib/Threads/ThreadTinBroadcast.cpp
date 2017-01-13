//
// Created by defacto on 01.01.17.
//

#include "ThreadTinBroadcast.h"
#include "Kernel/Kernel.h"

ThreadTinBroadcast::ThreadTinBroadcast(TinAddress broadcastAddress, Kernel &kernel) : ActionQueue(this, "BroadcastThread"), log("BroadcastThread"),
                                                                                      broadcastSocket(broadcastAddress), kernel(kernel){

}

void ThreadTinBroadcast::init() {
	log.debug("init start");
	try{
		broadcastSocket.initSocket();
		log.debug("init OK");
		kernel.add( [](Kernel &k){ k.broadcastThreadStartOk();});
	} catch( std::exception &e){
		log.warn("Init failed Exception ", e.what());
		kernel.add( [](Kernel &k){ k.broadcastThreadStartFailure();});
	}
}

void ThreadTinBroadcast::sendAnnounceMessage(std::vector<Resource> resources) {
	log.debug("sendAnnounceMessage with ", Help::writeVecContents(resources));
	try{
		broadcastSocket.sendAnnounceMessage(resources);
		log.debug("sendAnnounceMessage ok" );
	}catch ( std::exception &e){
		log.warn("sendAnnounceMessage failed. Exception ",e.what());
	}
}

void ThreadTinBroadcast::sendRevertMessage(std::vector<Resource> resources) {
	log.debug("sendRevertMessage with ",  Help::writeVecContents(resources));

	try{
		broadcastSocket.sendRevertMessage(resources);
		log.debug("sendRevertMessage ok");
	}catch ( std::exception &e){
		log.warn("sendRevertMessage failed. Exception is ",e.what());
	}
}

void ThreadTinBroadcast::internalKillYourself() {
	log.debug("got internalKillYourself. Telling broadcastSocket to shut down");
	broadcastSocket.shutdownSocket();
}
