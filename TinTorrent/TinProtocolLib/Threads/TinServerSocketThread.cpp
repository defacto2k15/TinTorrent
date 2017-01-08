//
// Created by defacto on 01.01.17.
//

#include "TinServerSocketThread.h"
#include <Kernel/Kernel.h>

TinServerSocketThread::TinServerSocketThread(Kernel &kernel) : ActionQueue(this), serverSocket( Constants::communicationPort), kernel(kernel){
}

void TinServerSocketThread::init() {
	std::cout << "ServerSocketThread: init started " <<std::endl;
	try{
		serverSocket.initSocket();
		std::cout << "ServerSocketThread: init OK" <<std::endl;
		kernel.add([](Kernel &k){ k.serverSocketInitOK();});
	} catch( std::exception &e){
		std::cout << Help::Str("ServerSocketThread: init FAILED ", e.what());
		kernel.add([](Kernel &k){ k.serverSocketInitFailed();});
	}
}

void TinServerSocketThread::listenForConnections() {
	std::cout << "ServerSocketThread: listening for connections "<<std::endl;
	while(true) {
		try{
			try {
				auto connectedSocket = serverSocket.listenForConnections();
				std::cout << Help::Str("ServerSocketThread: got connection from ", connectedSocket->getClientAddress())
				          << std::endl;
				kernel.add([connectedSocket](Kernel &k) { k.recievedConnection(connectedSocket); });
			} catch( SocketCommunicationException &ex ) {
				if (!threadShouldRun) {
					std::cout << "ServerSocketThread: Got SCE but thread is exiting " << std::endl;
					break;
				} else {
					throw ex;
				}
			}
		} catch (std::exception &e) {
			std::cout << Help::Str("ServerSocketThread: listening for connections failed ", e.what()) << std::endl;
		}
	}
}
