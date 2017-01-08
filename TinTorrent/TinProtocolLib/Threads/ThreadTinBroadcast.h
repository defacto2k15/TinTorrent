//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_THREADTINBROADCAST_H
#define TINTORRENT_THREADTINBROADCAST_H


#include <TinProtocolLib/TinBroadcastSocket.h>
#include <Multithreading/ActionQueue.h>

class Kernel;

class ThreadTinBroadcast : public ActionQueue<ThreadTinBroadcast>{
	TinBroadcastSocket broadcastSocket;
	Kernel &kernel;
public:
	ThreadTinBroadcast( TinAddress broadcastAddress, Kernel &kernel );

	void init();

	void sendAnnounceMessage( std::vector<Resource> resources );

	void sendRevertMessage( std::vector<Resource> resources );

	virtual  void internalKillYourself() override {
		std::cout << "BroadcastingThread: got internalKillYourself. Telling broadcastSocket to shut down"<< std::endl;
		broadcastSocket.shutdownSocket();
	}

};


#endif //TINTORRENT_THREADTINBROADCAST_H
