//
// Created by defacto on 01.01.17.
//

#include "TinServerSocketThread.h"
#include <Kernel/Kernel.h>

TinServerSocketThread::TinServerSocketThread(Kernel &kernel) : 
		ActionQueue(this, "ServerSocketThread"), log("ServerSocketThread"), serverSocket( Constants::communicationPort), kernel(kernel){
}

void TinServerSocketThread::init() {
	log.debug("init started ");
	try{
		serverSocket.initSocket();
		log.debug("init OK");
		kernel.add([](Kernel &k){ k.serverSocketInitOK();});
	} catch( std::exception &e){
		log.debug("init FAILED ", e.what());
		kernel.add([](Kernel &k){ k.serverSocketInitFailed();});
	}
}

void TinServerSocketThread::listenForConnections() {
	log.debug("listening for connections ");
	while(true) {
		try{
			try {
				auto connectedSocket = serverSocket.listenForConnections();
				log.debug("got connection from ", connectedSocket->getClientAddress());
				        
				kernel.add([connectedSocket](Kernel &k) { k.recievedConnection(connectedSocket); });
			} catch( SocketCommunicationException &ex ) {
				if (!threadShouldRun) {
					log.warn("Got SCE but thread is exiting ");
					break;
				} else {
					throw ex;
				}
			}
		} catch (std::exception &e) {
			log.warn("listening for connections failed ", e.what());
		}
	}
}

void TinServerSocketThread::internalKillYourself() {
	log.debug(" Got message internalKillYourself");
	serverSocket.shutdownSocket();
}
