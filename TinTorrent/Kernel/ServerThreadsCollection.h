//
// Created by defacto on 02.01.17.
//

#ifndef TINTORRENT_SERVERTHREADSCOLLECTION_H
#define TINTORRENT_SERVERTHREADSCOLLECTION_H


#include <memory>
#include <TinProtocolLib/Threads/TinConnectedServerThread.h>
#include <ProgramInfoProvider/outStructures/OutClientConnectionInfo.h>
#include <ProgramInfoProvider/outStructures/OutServerConnectionInfo.h>

class ServerThreadsCollection {
	int createdThreadsCount = 0;
	std::map<int, std::shared_ptr <TinConnectedServerThread>> threads;
public:
	void add(std::shared_ptr <TinConnectedServerThread> thread){
		createdThreadsCount++;
		threads[createdThreadsCount] = thread;
	}

	void closeThread(int threadId) {
		threads[threadId]->add( [](TinConnectedServerThread &c ){
			c.genericClose();
		});
		threads[threadId]->add( [](TinConnectedServerThread &c ){
			c.killYourself();
		});
		threads.erase(threadId);
	}

	std::shared_ptr<TinConnectedServerThread> get(int threadId) {
		return threads[threadId];
	}

	int getNextThreadId(){
		return createdThreadsCount + 1;
	}

	void closeThoseWorkingWith(Resource &resource) {
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

	std::vector<OutServerConnectionInfo> getConnectionsInfo(){
		std::vector<OutServerConnectionInfo> outVec;
		for( auto & pair : threads ){
			if( pair.second  && pair.second->getIsConnectionOpen()){
				outVec.push_back( OutServerConnectionInfo( pair.second->getConnectedAddress(), pair.second->getRequestedResource()));
			}
		}
		return outVec;
	}
};


#endif //TINTORRENT_SERVERTHREADSCOLLECTION_H
