//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_THREADTINUDPLISTENINGSOCKET_H
#define TINTORRENT_THREADTINUDPLISTENINGSOCKET_H


#include <TinProtocolLib/TinListeningUdpSocket.h>
#include <Kernel/Kernel.h>

class ThreadTinUdpListeningSocket : ActionQueue<ThreadTinUdpListeningSocket>{
	TinListeningUdpSocket udpSocket;
	Kernel &kernel;
public:
	ThreadTinUdpListeningSocket( Kernel &kernel) : ActionQueue(this), kernel(kernel){
	}

	void init(){
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

	void listenForBroadcasts(){
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

};


#endif //TINTORRENT_THREADTINUDPLISTENINGSOCKET_H
