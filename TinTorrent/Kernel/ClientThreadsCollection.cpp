//
// Created by defacto on 02.01.17.
//

#include "ClientThreadsCollection.h"

void ClientThreadsCollection::clearThread(TinAddress address) {
	clientThreads[address]->add( []( TinClientThread &t){
		t.genericCloseConnection();
	});
}

void ClientThreadsCollection::removeThread(TinAddress &address) {
	clientThreads[address]->add( []( TinClientThread &t){
		t.genericCloseConnection();
	});
	clientThreads[address]->add( []( TinClientThread &t){
		t.killYourself();
	});
	clientThreads.erase(address);
}

std::shared_ptr<TinClientThread> ClientThreadsCollection::get(TinAddress address) {
	return clientThreads[address];
}

void ClientThreadsCollection::closeThoseWorkingWith(Resource &resource) {
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

void ClientThreadsCollection::addNewThread(TinAddress &address, std::shared_ptr<TinClientThread> clientThread) {
	Assertions::check(clientThreads.count(address) == 0, Help::Str(" There arleady is one thread with address ", address) );
	clientThreads[address] = clientThread;
}

bool ClientThreadsCollection::isBusy(const TinAddress &address) {
	if( clientThreads.count(address) == 0 ){
		return true;
	} else if ( clientThreads[address]->hasOpenedConnection()){
		return false;
	}
	return true;
}

std::vector<OutClientConnectionInfo> ClientThreadsCollection::getConnectionsInfo() {
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
