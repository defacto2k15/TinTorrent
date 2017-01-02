//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_THREADTINBROADCAST_H
#define TINTORRENT_THREADTINBROADCAST_H


#include <TinProtocolLib/TinBroadcastSocket.h>
#include <Kernel/Kernel.h>

class ThreadTinBroadcast : ActionQueue<ThreadTinBroadcast>{
	TinBroadcastSocket broadcastSocket;
	Kernel &kernel;
public:
	ThreadTinBroadcast( TinAddress broadcastAddress, Kernel &kernel ) : ActionQueue(this),
	                                                                    broadcastSocket(broadcastAddress), kernel(kernel){

	}

	void init(){
		std::cout << "BroadcastingThread init start " << std::endl;
		try{
			broadcastSocket.initSocket();
			std::cout << " BroadcastingThread init OK " << std::endl;
			kernel.add( [](Kernel &k){ k.broadcastThreadStartOk();});
		} catch( std::exception &e){
			std::cout << Help::Str("BroadcastingThread. Init failed. Exception ", e.what()) << std::endl;
			kernel.add( [](Kernel &k){ k.broadcastThreadStartFailure();});
		}
	}

	void sendAnnounceMessage( std::vector<Resource> resources ){
		std::cout << "BroadcastingThread: sendAnnounceMessage with " << Help::writeVecContents(resources).str() << std::endl;
		try{
			broadcastSocket.sendAnnounceMessage(resources);
			std::cout << "BroadcastingThread: sendAnnounceMessage ok"<< std::endl;
		}catch ( std::exception &e){
			std::cout << "BroadcastingThread: sendAnnounceMessage failed"<< std::endl;
		}
	}

	void sendRevertMessage( std::vector<Resource> resources ){
		std::cout << "BroadcastingThread: sendRevertMessage with " << Help::writeVecContents(resources).str() << std::endl;
		try{
			broadcastSocket.sendRevertMessage(resources);
			std::cout << "BroadcastingThread: sendRevertMessage ok"<< std::endl;
		}catch ( std::exception &e){
			std::cout << "BroadcastingThread: sendRevertMessage failed"<< std::endl;
		}
	}

};


#endif //TINTORRENT_THREADTINBROADCAST_H
