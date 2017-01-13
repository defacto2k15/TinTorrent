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
	std::shared_ptr<TinClientThread> sharedPtr = clientThreads[address];
	clientThreads[address]->add( []( TinClientThread &t){
		t.genericCloseConnection();
	});
	clientThreads[address]->add( [sharedPtr]( TinClientThread &t){
		t.killYourself();
		sharedPtr.unique(); // to disable warn
	});
	clientThreads.erase(address);
	sharedPtr->join();
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

void ClientThreadsCollection::addNewThread(TinAddress &address, Kernel &kernel, std::function<void(TinClientThread&)> threadFunc) {
	Assertions::check(clientThreads.count(address) == 0, Help::Str(" There arleady is one thread with address ", address) );
	auto ptr = std::make_shared<TinClientThread>(address, kernel);
	clientThreads[address] = ptr;
	ptr->startThread();
	ptr->add(threadFunc);
}

bool ClientThreadsCollection::isBusy(const TinAddress &address) {
	return clientThreads.count(address) != 0;
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
