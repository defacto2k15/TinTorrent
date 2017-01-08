//
// Created by defacto on 01.01.17.
//

#include "Kernel.h"

Kernel::Kernel() : ActionQueue(this), log("Kernel"){
}

void Kernel::startApp(std::string workingDirectory, std::string configurationDirectory) {
	// set configuration
	log.debug("Starting logging");
	loggingMain.init();
	log.debug("setting configuration");
	loadConfiguration(configurationDirectory);

	/// read workingDirectory
	log.debug("Kernel starts with workingDir: ",workingDirectory);
	log.debug("Starting WorkingDirectoryManager");
	WorkingDirectoryManager directoryManager(workingDirectory);
	auto currentFiles = directoryManager.check();
	log.debug("Found ",currentFiles.size()," files in workDir: ", Help::writeVecContents(currentFiles).str());

	/// start of initialization
	programInfoProvider = std::make_unique<ProgramInfoProvider>(*this);

	log.debug("initialize working directory" );
	fileManagerThread = std::make_unique<FileManagerThread>(*this, workingDirectory);
	auto initialState = fileManagerThread->initialCheck();
	workingDirectoryState.init(initialState );
	fileManagerThread->startThread();
	fileManagerThread->startScheduledCheckings(Constants::secondsBetweenWorkingDirectoryUpdated);

	log.debug("creating & starting threads");
	threadTinBroadcast = std::make_unique<ThreadTinBroadcast>(TinAddress(Constants::broadcastingAddress, Constants::broadcastPort), *this);
	threadTinBroadcast->startThread();

	threadUdpListening = std::make_unique<ThreadTinUdpListeningSocket>(*this);
	threadUdpListening->startThread();

	serverSocketThread = std::make_unique<TinServerSocketThread>(*this);
	serverSocketThread->startThread();

	/// start listener of udp
	log.debug(" init threadTinBroadcast ");
	threadTinBroadcast->add( [](ThreadTinBroadcast & b){ b.init();});
	///  start thread
	this->startThread();
}

void Kernel::broadcastThreadStartOk() {
	log.debug(" broadcastThreadStartOk ");
	log.debug(" init threadUdpListening ");
	threadUdpListening->init();
}

void Kernel::broadcastThreadStartFailure() {
	log.warn(" broadcastThreadStartFailure ");
	killApplication();
}

void Kernel::udpListeningSocketInitOk() {
	log.debug(" udpListeningSocketInitOk ");
	log.debug(" init serverSocketThread ");
	serverSocketThread->init();
}

void Kernel::udpListeningSocketInitFailure() {
	log.warn(" udpListeningSocketInitFailure ");
	killApplication();
}

void Kernel::serverSocketInitOK() {
	log.debug(" serverSocketInitOK ");
	applicationInit2();
}

void Kernel::serverSocketInitFailed() {
	log.warn(" serverSocketInitFailed ");
	killApplication();
}

void Kernel::applicationInit2() {
	log.debug(" Told threads to listen for conenctions");
	threadUdpListening->add( []( ThreadTinUdpListeningSocket &t){  t.listenForBroadcasts();} );
	serverSocketThread->add( [] (TinServerSocketThread &t){ t.listenForConnections(); } );
	broadcastingThread = std::make_unique<ActionThread>( [this](){
		this->add([](Kernel &k) {k.broadcastResources();});
	}, Constants::secondsBetweenBroadcasts);
	broadcastingThread->start();
	downloadStartingThread = std::make_unique<ActionThread>([this](){
		this->add( [](Kernel &k){
			k.tryToDownloadResources();
		});
	}, Constants::secondsBetweenStartingDownloads);
	downloadStartingThread->start();
}

void Kernel::recievedBroadcast(std::pair<BroadcastMessage, TinAddress> pair) {
	log.debug(" RecievedBroadcast of ",pair);
	if( pair.first.getType() == BroadcastMessage::BroadcastType::ANNOUNCE) {
		tinNetworkState.addFiles(pair.second, pair.first.getResources());
		for (Resource &resource : pair.first.getResources()) {
			if (ContainerUtils::Contains(revertedResources, resource)) {
				log.debug(" resource  ", resource.toJson().dump(),
				      " is reverted will not download");
			} else if (workingDirectoryState.isDownloaded(resource)) {
				log.debug(" resource  ", resource.toJson().dump(), " is arleady downloaded");
			} else if (!workingDirectoryState.contains(resource)) {
				log.debug(" i want to start download of  ", resource.toJson().dump());
				resourcesToDownload.push_back(resource);
				workingDirectoryState.addEmptyResource(resource);
				fileManagerThread->createEmptyResource(resource);
			}
		}
		tryToDownloadResources();
	} else {
		for( auto &resource : pair.first.getResources()){
			removeRevertedResource(resource);
		}
	}
}

void Kernel::connectingToClientFailed(TinAddress address) {
	log.warn(" connectingToClientFailed  ",address);
	removeConnectionToClient(address);
}

void Kernel::gotBadResourceResponse(TinAddress addressToConnect,
                                    MessageResourceResponse::ResourceResponseValue responseValue) {
	log.debug(" gotBadResourceResponse Address: ",addressToConnect," response value ",responseValue.getValue());
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

void Kernel::clientCommunicationClosed(TinAddress &address, MessageClose::CloseReason closeReason) {
	log.debug(" clientCommunicationClosed Address: ",address, " due to ",closeReason);
	std::shared_ptr<TinClientThread> clientThread = clientThreads.get(address);
	workingDirectoryState.deallocateSegmentRange( clientThread->getRequestedResource(), clientThread->getRequestedSegments() );
	clientThreads.clearThread(address);
}

void Kernel::clientCommunicationFailure(TinAddress address) {
	log.warn(" clientCommunicationFailure Address: ",address);
	removeConnectionToClient(address);
}

void Kernel::recievedSegments(TinAddress address, Resource resource, SegmentsSet segmentsSet) {
	log.warn(" recievedSegments set: ",segmentsSet);
	fileManagerThread->add([&resource, &segmentsSet]( FileManagerThread &f){ f.setSegments(resource, segmentsSet);} );
	workingDirectoryState.setSegmentsAsDownloaded( resource, segmentsSet.getRange());

	std::shared_ptr<TinClientThread> clientThread = clientThreads.get(address);
	SegmentRange segmentsToDownload = workingDirectoryState.allocateSegmentsToDownload(resource);
	if( segmentsToDownload.empty()){
		log.debug(" no segments to download for client ",address," will now close it" );
		clientThread->add( [](TinClientThread &t){ t.closeConnection(MessageClose::CloseReason::OK);});
		clientThreads.clearThread(address);
	} else {
		log.debug(" telling ",address," to download ",segmentsToDownload );
		clientThread->add( [segmentsToDownload](TinClientThread &t){ t.recieveSegments(segmentsToDownload, MessageStartSendingRequest::PreviousStatus::OK);});
	}
}

void Kernel::recievedConnection(std::shared_ptr<TinConnectedServerSocket> connectedServerSocket) {
	log.debug("Recieved connection of connectedServerSocket  ", connectedServerSocket->getClientAddress());
	auto connectedServerThread = std::make_shared<TinConnectedServerThread>(*this, connectedServerSocket, serverThreads.getNextThreadId());
	serverThreads.add(connectedServerThread);
	connectedServerThread->add( []( TinConnectedServerThread &t){ t.listenForResourceRequest();});
}

void Kernel::gotResourceRequest(int threadId, Resource resource) {
	std::shared_ptr<TinConnectedServerThread> connectedThread = serverThreads.get(threadId);
	log.debug(" gotResourceRequest of ",threadId," resource ",resource);
	if( ContainerUtils::Contains(revertedResources, resource)){
		log.debug(" resource is reverted, will respond accordingly ");
		connectedThread->add( [](TinConnectedServerThread &t){ t.sendResourceResponse( MessageResourceResponse::ResourceResponseValue::REVERTED);});
		serverThreads.closeThread(threadId);
	} else if (!workingDirectoryState.isDownloaded(resource)){
		log.debug(" resource is missing ");
		connectedThread->add( [](TinConnectedServerThread &t){ t.sendResourceResponse( MessageResourceResponse::ResourceResponseValue::MISSING);});
		serverThreads.closeThread(threadId);
	} else {
		log.debug(" resource is present, will say accordingly ");
		connectedThread->add( [](TinConnectedServerThread &t){ t.sendResourceResponse( MessageResourceResponse::ResourceResponseValue::OK);});
	}
}

void Kernel::gotStartSendingRequest(int threadId, MessageStartSendingRequest startSendingRequest) {
	log.debug(" gotStartSendingRequest threadId ",threadId, " request ",startSendingRequest.toJson().dump());
	std::shared_ptr<TinConnectedServerThread> connectedThread = serverThreads.get(threadId);
	auto resource = connectedThread->getRequestedResource();
	if( ContainerUtils::Contains(revertedResources, resource)){
		log.debug(" resource is reverted, will respond accordingly ");
		connectedThread->add( [](TinConnectedServerThread &t ){ t.sendCloseMessage(MessageClose::CloseReason::RESOURCE_REVERTED);} );
		serverThreads.closeThread(threadId);
	} else if (!workingDirectoryState.isDownloaded(resource)){
		log.debug(" resource is missing ");
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

void Kernel::requestedSegmentProvided(int threadId, SegmentsSet segmentsSet) {
	log.debug(" requestedSegmentProvided of thread ",threadId, " segmentsSet is ",segmentsSet);
	std::shared_ptr<TinConnectedServerThread> connectedThread = serverThreads.get(threadId);
	connectedThread->add( [=]( TinConnectedServerThread &t ){
		t.sendSegments(segmentsSet);
	});
}

void Kernel::requestedResourceMissing(int threadId, Resource resource) {
	log.debug(" requestedResourceMissing of thread ",threadId, " resource  is ",resource);
	std::shared_ptr<TinConnectedServerThread> connectedThread = serverThreads.get(threadId);
	workingDirectoryState.removeResource(resource);
	connectedThread->add( [](TinConnectedServerThread &t){ t.sendCloseMessage( MessageClose::CloseReason::RESOURCE_MISSING);});
	serverThreads.closeThread(threadId);
}

void Kernel::serverCommunicationClosed(int threadId) {
	log.debug(" serverCommunicationClosed of thread ",threadId);
	serverThreads.closeThread(threadId);
}

void Kernel::serverCommunicationFailure(int threadId) {
	log.warn(" serverCommunicationFailure of thread ",threadId);
	serverThreads.closeThread(threadId);
}

void Kernel::broadcastResources() {
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

void Kernel::resourceMissing(Resource resource) {
	log.warn(" resourceMissing: ",resource);
	workingDirectoryState.removeResource(resource);
	clientThreads.closeThoseWorkingWith(resource);
	serverThreads.closeThoseWorkingWith(resource);
	ContainerUtils::remove( resourcesToDownload, resource);
	//todo this is temporary
	ContainerUtils::remove( revertedResources, resource);
}

void Kernel::workingDirectoryChanged(UpdateInfo updateInfo) {
	log.debug(" workingDirectoryChanged: ",updateInfo);
	for( FileInfo &info : updateInfo.getNewFiles()){
		Resource res = info.getResource();
		if( ContainerUtils::Contains(revertedResources, res)){
			log.debug(" : ", res, " is in reverted resources. Will be removed");
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

ProgramInfoProvider &Kernel::getProgramInfoProvider() {
	return *programInfoProvider;
}

void Kernel::closeKernel() {
	log.debug("Closing");
	killYourself();
	log.debug("Killing fileManagerThread ");
	fileManagerThread->killYourself();
	log.debug("Killing threadTinBroadcast");
	threadTinBroadcast->killYourself();
	log.debug("Killing threadUdpListening");
	threadUdpListening->killYourself();
	log.debug("Killing serverSocketThread");
	serverSocketThread->killYourself();
	log.debug("Killing broadcastingThread");
	broadcastingThread->killYourself();
	log.debug("Killing downloadStartingThread");
	downloadStartingThread->killYourself();

	log.debug("Joining fileManagerThread ");
	fileManagerThread->join();
	log.debug("Joining threadTinBroadcast ");
	threadTinBroadcast->join();
	log.debug("Joining threadUdpListening ");
	threadUdpListening->join();
	log.debug("Joining serverSocketThread ");
	serverSocketThread->join();
	log.debug("Joining broadcastingThread");
	broadcastingThread->join();
	log.debug("Joining downloadStaringThread");
	downloadStartingThread->join();

	log.debug("Kill my thread ");
	join();
}

WorkingDirectoryState &Kernel::getWorkingDirectoryState() {
	return workingDirectoryState;
}

TinNetworkState &Kernel::getTinNetworkState() {
	return tinNetworkState;
}

ClientThreadsCollection &Kernel::getClientThreads() {
	return clientThreads;
}

ServerThreadsCollection &Kernel::getServerThreads() {
	return serverThreads;
}

std::vector<Resource> Kernel::getResourcesToDownload() {
	return resourcesToDownload;
}

std::vector<Resource> Kernel::getRevertedResources() {
	return revertedResources;
}

void Kernel::loadConfiguration(std::string configurationDirectory) {
	if(configurationDirectory.empty()){
		log.debug("Configuration directory is empty - wont load file " );
		return;
	}
	std::ifstream t(configurationDirectory);
	if( !t){
		log.debug("Cannot read configuration file ",configurationDirectory," reverting to default") ;
	}
	std::string str((std::istreambuf_iterator<char>(t)),
	                std::istreambuf_iterator<char>());
	if(Constants::parseConstants(str)){
		log.debug("Loading configuration file ",configurationDirectory," OK") ;
	} else {
		log.debug("Failed processing configuration file ",configurationDirectory," reverting to default");
	}

}

void Kernel::tryToDownloadResources() {
	log.debug(" tryToDownloadResources ") ;
	for( auto &resource : resourcesToDownload){
		std::vector<TinAddress> clientsThanCanDownloadResource = tinNetworkState.avalibleClientsForDownload(resource);
		log.debug(" resource ",resource,
		      " can be downloaded from ",Help::writeVecContents(clientsThanCanDownloadResource).str()) ;
		for( auto &address : clientsThanCanDownloadResource ){
			if( clientThreads.isBusy(address)){
				log.debug(" resource ",resource,
				      " cannot be downloaded from ",address," as it is arleady downloading sth.") ;
				break;
			}

			SegmentRange segmentsToDownload = workingDirectoryState.allocateSegmentsToDownload(resource);
			if( segmentsToDownload.empty()){
				log.debug(" no segments to download for client ",address ) ;
				break;
			}
			log.debug(" will download segments ", segmentsToDownload, " by client ",address) ;
			auto newClientThread = std::make_shared<TinClientThread>(address, *this);
			newClientThread->add( [resource, segmentsToDownload](TinClientThread &clientThread ){
				clientThread.startDownloadingProcess(resource,segmentsToDownload);
			});
			clientThreads.addNewThread(address,newClientThread);
			newClientThread->startThread();
		}
	}
}

void Kernel::removeRevertedResource(Resource &resource) {
	tinNetworkState.removeResource(resource);
	workingDirectoryState.removeResource(resource);
	fileManagerThread->add([resource](FileManagerThread &f ){ f.removeResource(resource);});
	ContainerUtils::remove(resourcesToDownload, resource);
	revertedResources.push_back(resource);
}

void Kernel::removeConnectionToClient(TinAddress &address) {
	tinNetworkState.removeClient(address);
	std::shared_ptr<TinClientThread> clientThread = clientThreads.get(address);
	workingDirectoryState.deallocateSegmentRange( clientThread->getRequestedResource(), clientThread->getRequestedSegments() );
	clientThreads.removeThread(address);
}

void Kernel::killApplication() {
	log.info(" killing application." );

	std::exit(-1);
}
