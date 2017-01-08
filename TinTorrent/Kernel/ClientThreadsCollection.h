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

	void clearThread(TinAddress address);

	void removeThread (TinAddress &address);

	std::shared_ptr<TinClientThread> get(TinAddress address);

	void closeThoseWorkingWith(Resource &resource);

	void addNewThread(TinAddress &address, std::shared_ptr<TinClientThread> clientThread);

	bool isBusy(const TinAddress &address);

	std::vector<OutClientConnectionInfo> getConnectionsInfo();
};


#endif //TINTORRENT_CLIENTTHREADSCOLLECTION_H
