//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_TINSERVERSOCKETTHREAD_H
#define TINTORRENT_TINSERVERSOCKETTHREAD_H


#include <TinProtocolLib/TinServerSocket.h>
#include <Kernel/Kernel.h>
#include "TinConnectedServerThread.h"
#include <memory>

class TinServerSocketThread : ActionQueue<TinServerSocketThread>{
	TinServerSocket serverSocket;
	Kernel &kernel;
public:
	TinServerSocketThread(Kernel &kernel) : ActionQueue(this), serverSocket( Constants::communicationPort), kernel(kernel){
	}

	void init(){
		std::cout << "ServerSocketThread: init started " <<std::endl;
		try{
			serverSocket.initSocket();
			std::cout << "ServerSocketThread: init OK" <<std::endl;
			kernel.add([](Kernel &k){ k.serverSocketInitOK();});
		} catch( std::exception &e){
			std::cout << Help::Str("ServerSocketThread: init FAILED ", e);
			kernel.add([](Kernel &k){ k.serverSocketInitFailed();});
		}
	}

	void listenForConnections(){
		std::cout << "ServerSocketThread: listening for connections "<<std::endl;
		while(true) {
			try {
				auto connectedSocket = serverSocket.listenForConnections();
				std::cout << Help::Str("ServerSocketThread: got connection from ", connectedSocket->getClientAddress()) << std::endl;
				kernel.add([connectedSocket](Kernel &k) { k.recievedConnection(connectedSocket); });
			} catch (std::exception &e) {
				std::cout << Help::Str("ServerSocketThread: listening for connections failed ", e.what()) << std::endl;
			}
		}
	}
};


#endif //TINTORRENT_TINSERVERSOCKETTHREAD_H
