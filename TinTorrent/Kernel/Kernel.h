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
#include "LocalResourcesStateInfo.h"
#include <Kernel/Kernel.h>
#include <ProgramInfoProvider/outStructures/OutLocalResource.h>
#include <future>
#include <ProgramInfoProvider/ProgramInfoProvider.h>
#include <Multithreading/ActionThread.h>
#include <Logging/LoggingMain.h>

class Kernel : public ActionQueue<Kernel> {
	LoggingMain loggingMain;

	std::unique_ptr<ThreadTinBroadcast> threadTinBroadcast;
	std::unique_ptr<ThreadTinUdpListeningSocket> threadUdpListening;
	std::unique_ptr<TinServerSocketThread> serverSocketThread;
	std::unique_ptr<FileManagerThread> fileManagerThread;
	std::unique_ptr<ProgramInfoProvider> programInfoProvider;

	LocalResourcesStateInfo localResourcesStateInfo;
	TinNetworkState tinNetworkState;
	ClientThreadsCollection clientThreads;
	ServerThreadsCollection serverThreads;

	std::unique_ptr<ActionThread> broadcastingThread;
	std::unique_ptr<ActionThread> downloadStartingThread;

	std::unique_ptr<TinAddress> ourAddress;
public:
	LogBase log;

	Kernel();

	void startApp(std::string workingDirectory);


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

// view talking
	bool viewResouceReverted( Resource &resource );

///getters
	 WorkingDirectoryState &getWorkingDirectoryState();

	 TinNetworkState &getTinNetworkState();

	 ClientThreadsCollection &getClientThreads();

	 ServerThreadsCollection &getServerThreads();

	 std::vector<Resource> getResourcesThatAreNotYetDownloaded();

	 std::vector<Resource> getRevertedResources();

	 LocalResourcesStateInfo &getLocalResourcesStateInfo();

	void removeRevertedResource(Resource &resource);

	/// change state
	void orderResourceDownload(Resource &resource);
private:
	void tryToDownloadResources();

	void removeConnectionToClient( TinAddress &address);

	void addToDownload(Resource &resource);

	void killApplication();

	void setSignalHandler();

	static void sig_handler(int sig, siginfo_t * info, void * context);
};


#endif //TINTORRENT_KERNEL_H
