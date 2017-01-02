//
// Created by defacto on 02.01.17.
//

#ifndef TINTORRENT_SERVERTHREADSCOLLECTION_H
#define TINTORRENT_SERVERTHREADSCOLLECTION_H


#include <memory>
#include <TinProtocolLib/Threads/TinConnectedServerThread.h>

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
};


#endif //TINTORRENT_SERVERTHREADSCOLLECTION_H
