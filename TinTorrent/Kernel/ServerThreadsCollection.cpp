//
// Created by defacto on 02.01.17.
//

#include "ServerThreadsCollection.h"

void ServerThreadsCollection::add(std::shared_ptr<TinConnectedServerThread> thread) {
	createdThreadsCount++;
	threads[createdThreadsCount] = thread;
}

void ServerThreadsCollection::closeThread(int threadId) {
	threads[threadId]->add( [](TinConnectedServerThread &c ){
		c.genericClose();
	});
	threads[threadId]->add( [](TinConnectedServerThread &c ){
		c.killYourself();
	});
	threads.erase(threadId);
}

std::shared_ptr<TinConnectedServerThread> ServerThreadsCollection::get(int threadId) {
	return threads[threadId];
}

void ServerThreadsCollection::closeThoseWorkingWith(Resource &resource) {
	std::vector<int> threadsToClose;
	for( auto &pair  : threads ){
		if( pair.second->getRequestedResource() == resource){
			threadsToClose.push_back(pair.first);
		}
	}
	for( auto &id : threadsToClose){
		closeThread(id);
	}
}

std::vector<OutServerConnectionInfo> ServerThreadsCollection::getConnectionsInfo() {
	std::vector<OutServerConnectionInfo> outVec;
	for( auto & pair : threads ){
		if( pair.second  && pair.second->getIsConnectionOpen()){
			outVec.push_back( OutServerConnectionInfo( pair.second->getConnectedAddress(), pair.second->getRequestedResource()));
		}
	}
	return outVec;
}
