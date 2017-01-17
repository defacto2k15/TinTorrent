//
// Created by defacto on 02.01.17.
//

#include "ServerThreadsCollection.h"

void ServerThreadsCollection::add(std::shared_ptr<TinConnectedServerThread> thread) {
	createdThreadsCount++;
	threads[createdThreadsCount] = thread;
}

void ServerThreadsCollection::closeThread(int threadId) {
	std::shared_ptr<TinConnectedServerThread> keepAlivePtr = threads[threadId];
	threads[threadId]->add( [keepAlivePtr](TinConnectedServerThread &c ){
		c.killYourself();
		keepAlivePtr.unique();
	});
	threads.erase(threadId);
	keepAlivePtr->join();
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

int ServerThreadsCollection::getNextThreadId() {
	return createdThreadsCount + 1;
}

void ServerThreadsCollection::removeAllThreads() {
	std::vector<int> idsToRemove;
	for( auto &pair : threads ){
		idsToRemove.push_back(pair.first);
	}
	for( auto &id : idsToRemove){
		closeThread(id);
	}
}
