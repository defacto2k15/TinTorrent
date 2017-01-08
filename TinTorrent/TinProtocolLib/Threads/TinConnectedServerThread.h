//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_TINCONNECTEDSERVERTHREAD_H
#define TINTORRENT_TINCONNECTEDSERVERTHREAD_H


class Kernel;

#include <TinProtocolLib/TinConnectedServerSocket.h>
#include <Multithreading/ActionQueue.h>
#include <Common/SegmentsSet.h>

class TinConnectedServerThread : public ActionQueue<TinConnectedServerThread> {
	LogBase log;
	Kernel &kernel;
	std::shared_ptr<TinConnectedServerSocket> connectedServerSocket;
	int threadId;
	Resource requestedResource;
	bool isConnectionOpen = false;
public:
	TinConnectedServerThread( Kernel &kernel, std::shared_ptr<TinConnectedServerSocket> connectedServerSocket,
	                          int threadId);

	void listenForResourceRequest();

	void sendResourceResponse(MessageResourceResponse::ResourceResponseValue responseValue);

	void sendSegments( SegmentsSet segmentsSet );

	void listenForCloseMessage();

	void sendCloseMessage( MessageClose::CloseReason reason );

	void genericClose();

	int getThreadId();

	Resource getRequestedResource();

	TinAddress getConnectedAddress();

	virtual void internalKillYourself();

	bool getIsConnectionOpen();
private:
	void handleException( std::function<void()> func );
};


#endif //TINTORRENT_TINCONNECTEDSERVERTHREAD_H
