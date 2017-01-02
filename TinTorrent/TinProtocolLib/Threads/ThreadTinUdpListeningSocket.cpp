//
// Created by defacto on 01.01.17.
//
#include <Kernel/Kernel.h>
#include "ThreadTinUdpListeningSocket.h"

ThreadTinUdpListeningSocket::ThreadTinUdpListeningSocket(Kernel &kernel) : ActionQueue(this), kernel(kernel){
}

void ThreadTinUdpListeningSocket::init() {
	try{
		std::cout << "TinUdpListeningSocket init start "<< std::endl;
		udpSocket.initSocket();
		std::cout << "TinUdpListeningSocket init OK"<< std::endl;
		kernel.add([](Kernel &k){ k.udpListeningSocketInitOk();});
	} catch( std::exception &e){
		std::cout << Help::Str("TinUdpListeningSocket init failure ", e.what())<< std::endl;
		kernel.add([](Kernel &k){ k.udpListeningSocketInitFailure();});
	}
}

void ThreadTinUdpListeningSocket::listenForBroadcasts() {
	std::cout << "TinUdpListeningSocket. Starting listening for broadcasts "<< std::endl;
	while(true){ //todo wymysl cos
		try{
			auto pair = udpSocket.listenForMessages();
			std::cout << Help::Str("TinUdpListeningSocket. Got broadcastMessage ", pair ) <<std::endl;
			kernel.add( [pair](Kernel &k){ k.recievedBroadcast(pair);});
		} catch(std::exception &e ){
			std::cout << Help::Str("TinUdpListeningSocket. Listening failed ", e.what()) << std::endl;
		}
	}
}
