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

class Kernel : public ActionQueue<Kernel> {
	std::vector<FileInfo> currentFiles;
	std::unique_ptr<ThreadTinBroadcast> threadTinBroadcast;
	std::unique_ptr<ThreadTinUdpListeningSocket> threadUdpListening;
	std::unique_ptr<TinServerSocketThread> serverSocketThread;
	std::unique_ptr<FileManagerThread> fileManagerThread;

	WorkingDirectoryState workingDirectoryState;
	TinNetworkState tinNetworkState;
	ClientThreadsCollection clientThreads;
	ServerThreadsCollection serverThreads;

	std::vector<Resource> resourcesToDownload;
	std::vector<Resource> revertedResources;

	std::thread broadcastingThread;
	std::thread downloadStartingThread;
public:
	Kernel() : ActionQueue(this){
	}

	void startApp(std::string workingDirectory, std::string configurationDirectory){
		// set configuration
		std::cout << "Starting setting configuration" << std::endl;
		loadConfiguration(configurationDirectory);

		/// read workingDirectory
		std::cout << Help::Str("Kernel starts with workingDir: "+workingDirectory) << std::endl;
		std::cout << Help::Str("Starting WorkingDirectoryManager") << std::endl;
		WorkingDirectoryManager directoryManager(workingDirectory);
		currentFiles = directoryManager.check();
		std::cout << Help::Str("Found ",currentFiles.size()," files in workDir: ") << std::endl;
		std::cout << Help::writeVecContents(currentFiles).str() << std::endl;

		/// start of initialization
		std::cout << "Kernel: initialize working directory" << std::endl;
		fileManagerThread = std::make_unique<FileManagerThread>(*this, workingDirectory);
		auto initialState = fileManagerThread->initialCheck();
		workingDirectoryState.init(initialState );
		fileManagerThread->startThread();
		fileManagerThread->startScheduledCheckings(Constants::secondsBetweenWorkingDirectoryUpdated);

		std::cout << "Kernel: creating & starting threads" << std::endl;
		threadTinBroadcast = std::make_unique<ThreadTinBroadcast>(TinAddress(Constants::broadcastingAddress, Constants::broadcastPort), *this);
		threadTinBroadcast->startThread();

		threadUdpListening = std::make_unique<ThreadTinUdpListeningSocket>(*this);
		threadUdpListening->startThread();

		serverSocketThread = std::make_unique<TinServerSocketThread>(*this);
		serverSocketThread->startThread();

		/// start listener of udp
		std::cout << "Kernel: init threadTinBroadcast " <<std::endl;
		threadTinBroadcast->add( [](ThreadTinBroadcast & b){ b.init();});
		///  start thread
		this->startThread();
	}
// INIT 1.5 /////////////////////////////////
	void broadcastThreadStartOk(){
		std::cout << "Kernel: broadcastThreadStartOk " <<std::endl;
		std::cout << "Kernel: init threadUdpListening " <<std::endl;
		threadUdpListening->init();
	}

	void broadcastThreadStartFailure(){
		std::cout << "Kernel: broadcastThreadStartFailure " <<std::endl;
		killApplication();
	}

	void udpListeningSocketInitOk(){
		std::cout << "Kernel: udpListeningSocketInitOk " <<std::endl;
		std::cout << "Kernel: init serverSocketThread " <<std::endl;
		serverSocketThread->init();
	}

	void udpListeningSocketInitFailure(){
		std::cout << "Kernel: udpListeningSocketInitFailure " <<std::endl;
		killApplication();
	}

	void serverSocketInitOK(){
		std::cout << "Kernel: serverSocketInitOK " <<std::endl;
		applicationInit2();
	}

	void serverSocketInitFailed(){
		std::cout << "Kernel: serverSocketInitFailed " <<std::endl;
		killApplication();
	}
// INIT 2 //////////////////////////////////

	void applicationInit2(){
		std::cout << "Kernel: Told threads to listen for conenctions" <<std::endl;
		threadUdpListening->add( []( ThreadTinUdpListeningSocket &t){  t.listenForBroadcasts();} );
		serverSocketThread->add( [] (TinServerSocketThread &t){ t.listenForConnections(); } );
		broadcastingThread = std::thread( [this](){
			while(true){ //todo stop method
				this->add([](Kernel &k) {k.broadcastResources();});
				std::this_thread::sleep_for(std::chrono::seconds(Constants::secondsBetweenBroadcasts));
			}
		});
		downloadStartingThread = std::thread ([this](){
			while(true){
				this->add( [](Kernel &k){
					k.tryToDownloadResources();
				});
				std::this_thread::sleep_for(std::chrono::seconds(Constants::secondsBetweenStartingDownloads));
			}
		});
	}

/// STORY : TinClientThread recieved broadcast //////////////////////////
	void recievedBroadcast(std::pair<BroadcastMessage, TinAddress> pair){
		std::cout << Help::Str("Kernel: RecievedBroadcast of ",pair) << std::endl;
		if( pair.first.getType() == BroadcastMessage::BroadcastType::ANNOUNCE) {
			tinNetworkState.addFiles(pair.second, pair.first.getResources());
			for (Resource &resource : pair.first.getResources()) {
				if (ContainerUtils::Contains(revertedResources, resource)) {
					std::cout << Help::Str("Kernel: resource  ", resource.toJson().dump(),
					                       " is reverted will not download") << std::endl;
				} else if (workingDirectoryState.isDownloaded(resource)) {
					std::cout << Help::Str("Kernel: resource  ", resource.toJson().dump(), " is arleady downloaded")
					          << std::endl;
				} else if (!workingDirectoryState.contains(resource)) {
					std::cout << Help::Str("Kernel: i want to start download of  ", resource.toJson().dump())
					          << std::endl;
					resourcesToDownload.push_back(resource);
				}
			}
			tryToDownloadResources();
		} else {
			for( auto &resource : pair.first.getResources()){
				removeRevertedResource(resource);
			}
		}
	}

	void connectingToClientFailed(TinAddress address){
		std::cout << Help::Str("Kernel: connectingToClientFailed  ",address) << std::endl;
		removeConnectionToClient(address);
	}

	void gotBadResourceResponse( TinAddress addressToConnect, MessageResourceResponse::ResourceResponseValue responseValue ){
		std::cout << Help::Str("Kernel: gotBadResourceResponse Address: ",addressToConnect," response value ",responseValue.getValue()) << std::endl;
		std::shared_ptr<TinClientThread> clientThread = clientThreads.get(addressToConnect);
		auto resource =  clientThread->getRequestedResource();
		if( responseValue == MessageResourceResponse::ResourceResponseValue::REVERTED ){
			clientThreads.clearThread(addressToConnect);
			removeRevertedResource(resource);
		} else if ( responseValue == MessageResourceResponse::ResourceResponseValue::MISSING ){
			workingDirectoryState.deallocateSegmentRange( resource, clientThread->getRequestedSegments() );
			tinNetworkState.removeResourceFromClient( addressToConnect, resource);
		}
	}

	void clientCommunicationClosed( TinAddress &address, MessageClose::CloseReason closeReason){
		std::cout << Help::Str("Kernel: clientCommunicationClosed Address: ",address, " due to ",closeReason) << std::endl;
		std::shared_ptr<TinClientThread> clientThread = clientThreads.get(address);
		workingDirectoryState.deallocateSegmentRange( clientThread->getRequestedResource(), clientThread->getRequestedSegments() );
		clientThreads.clearThread(address);
	}

	void clientCommunicationFailure( TinAddress address){
		std::cout << Help::Str("Kernel: clientCommunicationFailure Address: ",address) << std::endl;
		removeConnectionToClient(address);
	}

	void recievedSegments(TinAddress address, Resource resource, SegmentsSet segmentsSet ){
		std::cout << Help::Str("Kernel: recievedSegments set: ",segmentsSet) << std::endl;
		fileManagerThread->add([&resource, &segmentsSet]( FileManagerThread &f){ f.setSegments(resource, segmentsSet);} );
		workingDirectoryState.setSegmentsAsDownloaded( resource, segmentsSet.getRange());

		std::shared_ptr<TinClientThread> clientThread = clientThreads.get(address);
		SegmentRange segmentsToDownload = workingDirectoryState.allocateSegmentsToDownload(resource);
		if( segmentsToDownload.empty()){
			std::cout << Help::Str("Kernel: no segments to download for client ",address," will now close it" ) << std::endl;
			clientThread->add( [](TinClientThread &t){ t.closeConnection(MessageClose::CloseReason::OK);});
			clientThreads.clearThread(address);
		} else {
			std::cout << Help::Str("Kernel: telling ",address," to download ",segmentsToDownload ) << std::endl;
			clientThread->add( [segmentsToDownload](TinClientThread &t){ t.recieveSegments(segmentsToDownload, MessageStartSendingRequest::PreviousStatus::OK);});
		}
	}

//// Story: Tin serverSocket
	void recievedConnection(std::shared_ptr<TinConnectedServerSocket> connectedServerSocket){
		auto connectedServerThread = std::make_shared<TinConnectedServerThread>(*this, connectedServerSocket, serverThreads.getNextThreadId());
		serverThreads.add(connectedServerThread);
		connectedServerThread->add( []( TinConnectedServerThread &t){ t.listenForResourceRequest();});
	}

	void gotResourceRequest( int threadId, Resource resource ){
		std::shared_ptr<TinConnectedServerThread> connectedThread = serverThreads.get(threadId);
		std::cout << Help::Str("Kernel: gotResourceRequest of ",threadId," resource ",resource) << std::endl;
		if( ContainerUtils::Contains(revertedResources, resource)){
			std::cout << Help::Str("Kernel: resource is reverted, will respond accordingly ") << std::endl;
			connectedThread->add( [](TinConnectedServerThread &t){ t.sendResourceResponse( MessageResourceResponse::ResourceResponseValue::REVERTED);});
			serverThreads.closeThread(threadId);
		} else if (!workingDirectoryState.isDownloaded(resource)){
			std::cout << Help::Str("Kernel: resource is missing ") << std::endl;
			connectedThread->add( [](TinConnectedServerThread &t){ t.sendResourceResponse( MessageResourceResponse::ResourceResponseValue::MISSING);});
			serverThreads.closeThread(threadId);
		} else {
			std::cout << Help::Str("Kernel: resource is present, will say accordingly ") << std::endl;
			connectedThread->add( [](TinConnectedServerThread &t){ t.sendResourceResponse( MessageResourceResponse::ResourceResponseValue::OK);});
		}
	}

	void gotStartSendingRequest(int threadId, MessageStartSendingRequest startSendingRequest){
		std::cout << Help::Str("Kernel: gotStartSendingRequest threadId ",threadId, " request ",startSendingRequest.toJson().dump()) << std::endl;
		std::shared_ptr<TinConnectedServerThread> connectedThread = serverThreads.get(threadId);
		auto resource = connectedThread->getRequestedResource();
		if( ContainerUtils::Contains(revertedResources, resource)){
			std::cout << Help::Str("Kernel: resource is reverted, will respond accordingly ") << std::endl;
			connectedThread->add( [](TinConnectedServerThread &t ){ t.sendCloseMessage(MessageClose::CloseReason::RESOURCE_REVERTED);} );
			serverThreads.closeThread(threadId);
		} else if (!workingDirectoryState.isDownloaded(resource)){
			std::cout << Help::Str("Kernel: resource is missing ") << std::endl;
			connectedThread->add( [](TinConnectedServerThread &t){ t.sendCloseMessage( MessageClose::CloseReason::RESOURCE_MISSING);});
			serverThreads.closeThread(threadId);
		}else {
			auto segmentInfo = startSendingRequest.getSegmentInfo();
			fileManagerThread->add( [threadId, &resource, segmentInfo]( FileManagerThread &t){
				Resource res = resource;
				t.requestSegments(threadId, res, segmentInfo);
			});
		}
	}

	void requestedSegmentProvided(int threadId, SegmentsSet segmentsSet ){
		std::cout << Help::Str("Kernel: requestedSegmentProvided of thread ",threadId, " segmentsSet is ",segmentsSet) << std::endl;
		std::shared_ptr<TinConnectedServerThread> connectedThread = serverThreads.get(threadId);
		connectedThread->add( [=]( TinConnectedServerThread &t ){
			t.sendSegments(segmentsSet);
		});
	}

	void requestedResourceMissing( int threadId, Resource resource ){
		std::cout << Help::Str("Kernel: requestedResourceMissing of thread ",threadId, " resource  is ",resource) << std::endl;
		std::shared_ptr<TinConnectedServerThread> connectedThread = serverThreads.get(threadId);
		workingDirectoryState.removeResource(resource);
		connectedThread->add( [](TinConnectedServerThread &t){ t.sendCloseMessage( MessageClose::CloseReason::RESOURCE_MISSING);});
		serverThreads.closeThread(threadId);
	}

	void serverCommunicationClosed( int threadId ){
		std::cout << Help::Str("Kernel: serverCommunicationClosed of thread ",threadId) << std::endl;
		serverThreads.closeThread(threadId);
	}

	void serverCommunicationFailure( int threadId ){
		std::cout << Help::Str("Kernel: serverCommunicationFailure of thread ",threadId) << std::endl;
		serverThreads.closeThread(threadId);
	}

/// broadcasting
	void broadcastResources(){
		auto downloadedResources = workingDirectoryState.getDownloadedResources();
		if( !downloadedResources.empty()){
			threadTinBroadcast->add( [this, downloadedResources]( ThreadTinBroadcast &b ){
				b.sendAnnounceMessage(downloadedResources);
			});
		}
		if( !revertedResources.empty()){
			threadTinBroadcast->add( [this] ( ThreadTinBroadcast &b){
				b.sendRevertMessage(revertedResources);
			});
		}
	}

/// FileProblems

	void resourceMissing( Resource resource ){
		std::cout << Help::Str("Kernel: resourceMissing: ",resource) << std::endl;
		workingDirectoryState.removeResource(resource);
		clientThreads.closeThoseWorkingWith(resource);
		serverThreads.closeThoseWorkingWith(resource);
		ContainerUtils::remove( resourcesToDownload, resource);
		//todo this is temporary
		ContainerUtils::remove( revertedResources, resource);
	}

	void workingDirectoryChanged( UpdateInfo updateInfo){
		std::cout << Help::Str("Kernel: workingDirectoryChanged: ",updateInfo) << std::endl;
		for( FileInfo &info : updateInfo.getNewFiles()){
			Resource res = info.getResource();
			if( ContainerUtils::Contains(revertedResources, res)){
				std::cout << Help::Str("Kernel: : ", res, " is in reverted resources. Will be removed") << std::endl;
				fileManagerThread->add([res](FileManagerThread &t ){
					t.removeResource(res);
				});
			} else {
				workingDirectoryState.addResource(res);
			}
		}
		for( FileInfo &info : updateInfo.getDeletedFiles() ){
			resourceMissing(info.getResource());
		}
		tryToDownloadResources();
	}
private:
	void loadConfiguration(std::string configurationDirectory){
		if(configurationDirectory.empty()){
			std::cout << "Configuration directory is empty - wont load file " << std::endl;
			return;
		}
		std::ifstream t(configurationDirectory);
		if( !t){
			std::cout << Help::Str("Cannot read configuration file ",configurationDirectory," reverting to default") << std::endl;
		}
		std::string str((std::istreambuf_iterator<char>(t)),
		                std::istreambuf_iterator<char>());
		if(Constants::parseConstants(str)){
			std::cout << Help::Str("Loading configuration file ",configurationDirectory," OK") << std::endl;
		} else {
			std::cout << Help::Str("Failed processing configuration file ",configurationDirectory," reverting to default") << std::endl;
		}

	}

	void tryToDownloadResources(){
		std::cout << Help::Str("Kernel: tryToDownloadResources ") << std::endl;
		for( auto &resource : resourcesToDownload){
			std::vector<TinAddress> clientsThanCanDownloadResource = tinNetworkState.avalibleClientsForDownload(resource);
			std::cout << Help::Str("Kernel: resource ",resource,
			                       " can be downloaded from ",Help::writeVecContents(clientsThanCanDownloadResource).str()) << std::endl;
			for( auto &address : clientsThanCanDownloadResource ){
				SegmentRange segmentsToDownload = workingDirectoryState.allocateSegmentsToDownload(resource);
				if( segmentsToDownload.empty()){
					std::cout << Help::Str("Kernel: no segments to download for client ",address ) << std::endl;
					break;
				}
				std::cout << Help::Str("Kernel: will download segments ", segmentsToDownload, " by client ",address) << std::endl;
				auto newClientThread = std::make_shared<TinClientThread>(address, *this);
				newClientThread->add( [resource, segmentsToDownload](TinClientThread &clientThread ){
					clientThread.startDownloadingProcess(resource,segmentsToDownload);
				});
				clientThreads.addNewThread(address,newClientThread);
				newClientThread->startThread();
			}
		}
	}


	void removeRevertedResource(Resource &resource){
		tinNetworkState.removeResource(resource);
		workingDirectoryState.removeResource(resource);
		fileManagerThread->add([resource](FileManagerThread &f ){ f.removeResource(resource);});
		ContainerUtils::remove(resourcesToDownload, resource);
		revertedResources.push_back(resource);
	}

	void removeConnectionToClient( TinAddress &address){
		tinNetworkState.removeClient(address);
		std::shared_ptr<TinClientThread> clientThread = clientThreads.get(address);
		workingDirectoryState.deallocateSegmentRange( clientThread->getRequestedResource(), clientThread->getRequestedSegments() );
		clientThreads.removeThread(address);
	}

	void killApplication(){
		std::cout << "Kernel: killing aplciation " << std::endl;
		std::exit(-1);
	}
};


#endif //TINTORRENT_KERNEL_H
