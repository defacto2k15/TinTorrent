//
// Created by defacto on 02.01.17.
//

#ifndef TINTORRENT_CLIENTTHREADSCOLLECTION_H
#define TINTORRENT_CLIENTTHREADSCOLLECTION_H


#include <ModelEntities/TinAddress.h>
#include <TinProtocolLib/Threads/TinClientThread.h>

class ClientThreadsCollection {
	std::map<TinAddress, std::shared_ptr<TinClientThread>, TinAddressCompare> clientThreads;
public:

	void clearThread(TinAddress address){
		clientThreads[address]->add( []( TinClientThread &t){
			t.genericCloseConnection();
		});
		clientThreads.erase(address);
	}

	std::shared_ptr<TinClientThread> get(TinAddress address) {
		return clientThreads[address];
	}

	void closeThoseWorkingWith(Resource &resource) {
		std::vector<TinAddress> thoseWorkingWith;
		for( auto &pair : clientThreads){
			if( pair.second->getRequestedResource() == resource){
				thoseWorkingWith.push_back(pair.first);
			}
		}
		for( auto &address : thoseWorkingWith){
			clearThread(address);
		}
	}

	void addNewThread(TinAddress &address, std::shared_ptr<TinClientThread> clientThread) {
		Assertions::check(clientThreads.count(address) == 0, Help::Str(" There arleady is one thread with address ", address) );
		clientThreads[address] = clientThread;
	}
};


#endif //TINTORRENT_CLIENTTHREADSCOLLECTION_H
