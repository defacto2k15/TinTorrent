//
// Created by defacto on 02.01.17.
//

#ifndef TINTORRENT_CLIENTTHREADSCOLLECTION_H
#define TINTORRENT_CLIENTTHREADSCOLLECTION_H


#include <ModelEntities/TinAddress.h>
#include <TinProtocolLib/Threads/TinClientThread.h>
#include <ProgramInfoProvider/outStructures/OutClientConnectionInfo.h>

class ClientThreadsCollection {
	std::map<TinAddress, std::shared_ptr<TinClientThread>, TinAddressCompare> clientThreads;
public:

	void clearThread(TinAddress address){
		clientThreads[address]->add( []( TinClientThread &t){
			t.genericCloseConnection();
		});
	}

	void removeThread (TinAddress &address){
		clientThreads[address]->add( []( TinClientThread &t){
			t.genericCloseConnection();
		});
		clientThreads[address]->add( []( TinClientThread &t){
			t.killYourself();
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

	bool isBusy(const TinAddress &address){
		if( clientThreads.count(address) == 0 ){
			return true;
		} else if ( clientThreads[address]->hasOpenedConnection()){
			return false;
		}
		return true;
	}

	std::vector<OutClientConnectionInfo> getConnectionsInfo(){
		std::vector<OutClientConnectionInfo> outVec;
		for( auto &pair : clientThreads ){
			if( isBusy(pair.first)){
				outVec.push_back( OutClientConnectionInfo(pair.first, pair.second->getRequestedResource()));
			} else {
				outVec.push_back( OutClientConnectionInfo(pair.first));
			}
		}
		return outVec;
	}
};


#endif //TINTORRENT_CLIENTTHREADSCOLLECTION_H
