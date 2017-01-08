//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_KERNEL_H
#define TINTORRENT_KERNEL_H


#include <FileManager/WorkingDirectoryManager.h>
#include <TinProtocolLib/Messages/BroadcastMessage.h>
#include <ModelEntities/TinAddress.h>
#include <Multithreading/ActionQueue.h>
#include <TinProtocolLib/Threads/ThreadTinBroadcast.h>
#include <TinProtocolLib/Threads/ThreadTinUdpListeningSocket.h>
#include <TinProtocolLib/Threads/TinServerSocketThread.h>
#include <FileManager/FileManagerThread.h>
#include <TinProtocolLib/Threads/TinClientThread.h>
#include "WorkingDirectoryState.h"
#include "TinNetworkState.h"
#include "ClientThreadsCollection.h"
#include "ServerThreadsCollection.h"
#include <Kernel/Kernel.h>
#include <ProgramInfoProvider/outStructures/OutLocalResource.h>
#include <future>
#include <ProgramInfoProvider/ProgramInfoProvider.h>
#include <Multithreading/ActionThread.h>
#include <Logging/LoggingMain.h>

class Kernel : public ActionQueue<Kernel> {
	LoggingMain loggingMain;
	LogBase log;
	std::unique_ptr<ThreadTinBroadcast> threadTinBroadcast;
	std::unique_ptr<ThreadTinUdpListeningSocket> threadUdpListening;
	std::unique_ptr<TinServerSocketThread> serverSocketThread;
	std::unique_ptr<FileManagerThread> fileManagerThread;
	std::unique_ptr<ProgramInfoProvider> programInfoProvider;

	WorkingDirectoryState workingDirectoryState;
	TinNetworkState tinNetworkState;
	ClientThreadsCollection clientThreads;
	ServerThreadsCollection serverThreads;

	std::vector<Resource> resourcesToDownload;
	std::vector<Resource> revertedResources;

	std::unique_ptr<ActionThread> broadcastingThread;
	std::unique_ptr<ActionThread> downloadStartingThread;
public:
	Kernel();

	void startApp(std::string workingDirectory, std::string configurationDirectory);
// INIT 1.5 /////////////////////////////////
	void broadcastThreadStartOk();

	void broadcastThreadStartFailure();

	void udpListeningSocketInitOk();

	void udpListeningSocketInitFailure();

	void serverSocketInitOK();

	void serverSocketInitFailed();
// INIT 2 //////////////////////////////////

	void applicationInit2();

/// STORY : TinClientThread recieved broadcast //////////////////////////
	void recievedBroadcast(std::pair<BroadcastMessage, TinAddress> pair);

	void connectingToClientFailed(TinAddress address);

	void gotBadResourceResponse( TinAddress addressToConnect, MessageResourceResponse::ResourceResponseValue responseValue );

	void clientCommunicationClosed( TinAddress &address, MessageClose::CloseReason closeReason);

	void clientCommunicationFailure( TinAddress address);

	void recievedSegments(TinAddress address, Resource resource, SegmentsSet segmentsSet );

//// Story: Tin serverSocket
	void recievedConnection(std::shared_ptr<TinConnectedServerSocket> connectedServerSocket);

	void gotResourceRequest( int threadId, Resource resource );

	void gotStartSendingRequest(int threadId, MessageStartSendingRequest startSendingRequest);

	void requestedSegmentProvided(int threadId, SegmentsSet segmentsSet );

	void requestedResourceMissing( int threadId, Resource resource );

	void serverCommunicationClosed( int threadId );

	void serverCommunicationFailure( int threadId );

/// broadcasting
	void broadcastResources();

/// FileProblems

	void resourceMissing( Resource resource );

	void workingDirectoryChanged( UpdateInfo updateInfo);

/// providing resource data
	ProgramInfoProvider &getProgramInfoProvider();

/// closing
	void closeKernel();

///getters
	 WorkingDirectoryState &getWorkingDirectoryState();

	 TinNetworkState &getTinNetworkState();

	 ClientThreadsCollection &getClientThreads();

	 ServerThreadsCollection &getServerThreads();

	 std::vector<Resource> getResourcesToDownload();

	 std::vector<Resource> getRevertedResources();

private:
	void loadConfiguration(std::string configurationDirectory);

	void tryToDownloadResources();


	void removeRevertedResource(Resource &resource);

	void removeConnectionToClient( TinAddress &address);

	void killApplication();
};


#endif //TINTORRENT_KERNEL_H
