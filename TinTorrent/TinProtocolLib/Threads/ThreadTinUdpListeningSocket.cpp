//
// Created by defacto on 01.01.17.
//
#include <Kernel/Kernel.h>
#include "ThreadTinUdpListeningSocket.h"

ThreadTinUdpListeningSocket::ThreadTinUdpListeningSocket(Kernel &kernel) 
		: ActionQueue(this, "TinUdpListeningSocket"), log("TinUdpListeningSocket"), kernel(kernel){
}

void ThreadTinUdpListeningSocket::init() {
	try{
		log.debug(" init start ");
		udpSocket.initSocket();
		log.debug(" init OK");
		kernel.add([](Kernel &k){ k.udpListeningSocketInitOk();});
	} catch( std::exception &e){
		log.debug(" init failure ", e.what());
		kernel.add([](Kernel &k){ k.udpListeningSocketInitFailure();});
	}
}

void ThreadTinUdpListeningSocket::listenForBroadcasts() {
	log.debug(". Starting listening for broadcasts ");
	while(true){
		try{
			auto pair = udpSocket.listenForMessages();
			log.debug(" Got broadcastMessage ", pair ) ;
			kernel.add( [pair](Kernel &k){ k.recievedBroadcast(pair);});
		} catch(SocketCommunicationException &ex){
			log.warn(" Listening failed. Got SCE", ex.what()) ;
			if( !threadShouldRun){
				log.info(" thread should stop, I am stopping loop" );
				break;
			}
		} catch(std::exception &e ){
			log.warn(" Listening failed ", e.what()) ;
		}
	}
}

void ThreadTinUdpListeningSocket::internalKillYourself() {
	log.debug(" Got internal kill yourself message. Shutdown of socket");
	udpSocket.shutdownSocket();
}
