//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_TINSERVERSOCKETTHREAD_H
#define TINTORRENT_TINSERVERSOCKETTHREAD_H


#include <TinProtocolLib/TinServerSocket.h>
#include "TinConnectedServerThread.h"
#include <memory>
class Kernel;

class TinServerSocketThread : public ActionQueue<TinServerSocketThread>{
	LogBase log;
	TinServerSocket serverSocket;
	Kernel &kernel;
public:
	TinServerSocketThread(Kernel &kernel);

	void init();

	void listenForConnections();

	void internalKillYourself() override;
};


#endif //TINTORRENT_TINSERVERSOCKETTHREAD_H
