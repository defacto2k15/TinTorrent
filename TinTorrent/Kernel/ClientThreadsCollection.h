//
// Created by defacto on 02.01.17.
//

#ifndef TINTORRENT_CLIENTTHREADSCOLLECTION_H
#define TINTORRENT_CLIENTTHREADSCOLLECTION_H


#include <ModelEntities/TinAddress.h>
#include <TinProtocolLib/Threads/TinClientThread.h>

class ClientThreadsCollection {
	std::map<TinAddress, std::shared_ptr<TinClientThread>> clientThreads;
public:

};


#endif //TINTORRENT_CLIENTTHREADSCOLLECTION_H
