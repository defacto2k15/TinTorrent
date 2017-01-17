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
	void add(std::shared_ptr <TinConnectedServerThread> thread);

	void closeThread(int threadId);

	std::shared_ptr<TinConnectedServerThread> get(int threadId);

	int getNextThreadId();

	void closeThoseWorkingWith(Resource &resource);

	std::vector<OutServerConnectionInfo> getConnectionsInfo();

	void removeAllThreads();
};


#endif //TINTORRENT_SERVERTHREADSCOLLECTION_H
