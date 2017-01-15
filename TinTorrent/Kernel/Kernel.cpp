//
// Created by defacto on 01.01.17.
//

#include "Kernel.h"

Kernel::Kernel() : ActionQueue(this, "Kernel"), log("Kernel"){
}

void Kernel::startApp(std::string workingDirectory) {
	/// read workingDirectory
	log.debug("Kernel starts with workingDir: ",workingDirectory);

	setSignalHandler();
	// logging
	log.debug("Starting logging");
	loggingMain.init();

	/// start of initialization
	programInfoProvider = std::make_unique<ProgramInfoProvider>(*this);

	log.debug("initialize working directory" );
	fileManagerThread = std::make_unique<FileManagerThread>(*this, workingDirectory);
	auto initialState = fileManagerThread->initialCheck();
	localResourcesStateInfo.init(initialState );
	log.debug("Found ",initialState.size()," files in workDir: ", Help::writeVecContents(initialState));
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
	}, Constants::secondsBetweenBroadcasts, "BroadcastingThread") ;
	broadcastingThread->start();
	downloadStartingThread = std::make_unique<ActionThread>([this](){
		this->add( [](Kernel &k){
			k.tryToDownloadResources();
		});
	}, Constants::secondsBetweenStartingDownloads, "DownloadStartThread");
	downloadStartingThread->start();
}

void Kernel::recievedBroadcast(std::pair<BroadcastMessage, TinAddress> pair) {
	log.debug(" RecievedBroadcast of ",pair);
	tinNetworkState.addAddress(pair.second);
	if( pair.first.getType() == BroadcastMessage::BroadcastType::ANNOUNCE) {
		for (Resource &resource : pair.first.getResources()) {
			if ( localResourcesStateInfo.isReverted(resource)) {
				log.debug(" resource  ", resource.toJson().dump(),
				          " is reverted will not download");
			}else{
				tinNetworkState.addResource(pair.second, resource);
				if (localResourcesStateInfo.isDownloaded(resource)) {
					log.debug(" resource  ", resource.toJson().dump(), " is arleady downloaded");
				} else {
					if( resource == Resource()){
						std::cerr << "TODOXXX222 CULPRIT OF BAD RESOURCCE " << std::endl;
					}

					localResourcesStateInfo.setToDownload(resource);
					if (!localResourcesStateInfo.containsLocalResource(resource)) {
						log.debug(" Can I  start download of  ", resource.toJson().dump());
						if( Constants::automaticDownload ){
							log.debug("Ok, i can do it");
							localResourcesStateInfo.addEmptyLocalResource(resource);
						} else {
							log.debug("Nope, start is non-automatic");
						}
						fileManagerThread->createEmptyResource(resource);
					}
				}
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
	clientThreads.removeThread(addressToConnect);
	if( responseValue == MessageResourceResponse::ResourceResponseValue::REVERTED ){
		removeRevertedResource(resource);
	} else if ( responseValue == MessageResourceResponse::ResourceResponseValue::MISSING ){
		localResourcesStateInfo.deallocateSegmentRange( resource, clientThread->getRequestedSegments() );
		tinNetworkState.removeResourceFromClient( addressToConnect, resource);
	}
}

void Kernel::clientCommunicationClosed(TinAddress &address, MessageClose::CloseReason closeReason) {
	log.debug(" clientCommunicationClosed Address: ",address, " due to ",closeReason);
	std::shared_ptr<TinClientThread> clientThread = clientThreads.get(address);
	localResourcesStateInfo.deallocateSegmentRange( clientThread->getRequestedResource(), clientThread->getRequestedSegments() );
	clientThreads.removeThread(address);
}

void Kernel::clientCommunicationFailure(TinAddress address) {
	log.warn(" clientCommunicationFailure Address: ",address);
	removeConnectionToClient(address);
}

void Kernel::recievedSegments(TinAddress address, Resource resource, SegmentsSet segmentsSet) {
	log.warn(" recievedSegments set: ",segmentsSet);
	if( !localResourcesStateInfo.containsLocalResource(resource)){
		log.warn("Got request when there is no localResource in info. Propably some racecondition");
	}
	fileManagerThread->add([resource, segmentsSet]( FileManagerThread &f)mutable{
		f.setSegments(resource, segmentsSet);
	} );

	localResourcesStateInfo.setSegmentsAsDownloaded( resource, segmentsSet.getRange());

	std::shared_ptr<TinClientThread> clientThread = clientThreads.get(address);
	SegmentRange segmentsToDownload = localResourcesStateInfo.allocateSegmentsToDownload(resource);
	if( segmentsToDownload.empty()){
		log.debug(" no segments to download for client ",address," will now close it" );
		clientThread->add( [](TinClientThread &t){ t.closeConnection(MessageClose::CloseReason::OK);});
		clientThreads.removeThread(address); //todo close with ok
		localResourcesStateInfo.setAsDownloaded(resource);

		log.debug("Download of resource ",resource," completed " );
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
	connectedServerThread->startThread();
}

void Kernel::gotResourceRequest(int threadId, Resource resource) {
	std::shared_ptr<TinConnectedServerThread> connectedThread = serverThreads.get(threadId);
	log.debug(" gotResourceRequest of ",threadId," resource ",resource);
	if( localResourcesStateInfo.isReverted(resource)){
		log.debug(" resource is reverted, will respond accordingly ");
		connectedThread->add( [](TinConnectedServerThread &t){ t.sendResourceResponse( MessageResourceResponse::ResourceResponseValue::REVERTED);});
		serverThreads.closeThread(threadId);
	} else if (!localResourcesStateInfo.isDownloaded(resource)){
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
	if( localResourcesStateInfo.isReverted(resource)){
		log.debug(" resource is reverted, will respond accordingly ");
		connectedThread->add( [](TinConnectedServerThread &t ){ t.sendCloseMessage(MessageClose::CloseReason::RESOURCE_REVERTED);} );
		serverThreads.closeThread(threadId);
	} else if (!localResourcesStateInfo.isDownloaded(resource)){
		log.debug(" resource is missing ");
		connectedThread->add( [](TinConnectedServerThread &t){ t.sendCloseMessage( MessageClose::CloseReason::RESOURCE_MISSING);});
		serverThreads.closeThread(threadId);
	}else {
		auto segmentInfo = startSendingRequest.getSegmentInfo();
		fileManagerThread->add( [=]( FileManagerThread &t){
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
	localResourcesStateInfo.removeLocalResource(resource);
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
	log.debug("Broadcasting resources ");
	auto downloadedResources = localResourcesStateInfo.getBroadcastableResources();
	if( !downloadedResources.empty()){
		threadTinBroadcast->add( [this, downloadedResources]( ThreadTinBroadcast &b ){
			b.sendAnnounceMessage(downloadedResources);
		});
	}
	auto reverted = localResourcesStateInfo.getReverted();
	if( !reverted.empty()){
		threadTinBroadcast->add( [this, reverted] ( ThreadTinBroadcast &b){
			b.sendRevertMessage(reverted);
		});
	}
}

void Kernel::resourceMissing(Resource resource) {
	log.warn(" resourceMissing: ",resource);
	localResourcesStateInfo.removeLocalResource(resource);
	clientThreads.closeThoseWorkingWith(resource);
	serverThreads.closeThoseWorkingWith(resource);
}

void Kernel::workingDirectoryChanged(UpdateInfo updateInfo) {
	log.debug(" workingDirectoryChanged: ",updateInfo);
	for( FileInfo &info : updateInfo.getNewFiles()){

		Resource res = info.getResource();
		if( res == Resource()){
			std::cerr << "TODOXXX CULPRIT OF BAD RESOURCCE " << std::endl;
		}
		if( localResourcesStateInfo.isReverted(res)){
			log.debug(" : ", res, " is in reverted resources. Will be removed");
			fileManagerThread->add([res](FileManagerThread &t ){
				t.removeResource(res);
			});
		} else {
			localResourcesStateInfo.addNewlyFoundLocalResource(res);
		}
	}
	for( FileInfo &info : updateInfo.getDeletedFiles() ){
		resourceMissing(info.getResource());
	}
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
	log.debug("Joining ClientThreads");
	clientThreads.removeAllThreads();
	log.debug("Joining ServerThreads");
	serverThreads.removeAllThreads();

	log.debug("Kill my thread ");
	join();
}

WorkingDirectoryState &Kernel::getWorkingDirectoryState() {
	return localResourcesStateInfo.getWorkingDirectoryState();
}

TinNetworkState &Kernel::getTinNetworkState() {
	log.debug("TIN NETWORK SETATE");
	return tinNetworkState;
}

ClientThreadsCollection &Kernel::getClientThreads() {
	return clientThreads;
}

ServerThreadsCollection &Kernel::getServerThreads() {
	return serverThreads;
}

std::vector<Resource> Kernel::getResourcesToDownload() {
	return localResourcesStateInfo.getResourcesToDownload();
}

std::vector<Resource> Kernel::getRevertedResources() {
	return localResourcesStateInfo.getReverted();
}

LocalResourcesStateInfo &Kernel::getLocalResourcesStateInfo(){
	return localResourcesStateInfo;
}

void Kernel::tryToDownloadResources() {
	log.debug(" tryToDownloadResources ") ;
	for( auto &resource : localResourcesStateInfo.getResourcesToDownload()){
		std::vector<TinAddress> clientsThanCanDownloadResource = tinNetworkState.avalibleClientsForDownload(resource);
		log.debug(" resource ",resource,
		      " can be obtained from addresses: ",Help::writeVecContents(clientsThanCanDownloadResource)) ;
		for( auto &address : clientsThanCanDownloadResource ){
			if( clientThreads.isBusy(address)){
				log.debug(" resource ",resource,
				      " cannot be downloaded from ",address," as it is arleady downloading sth.") ;
				break;
			}

			SegmentRange segmentsToDownload = localResourcesStateInfo.allocateSegmentsToDownload(resource);
			if( segmentsToDownload.empty()){
				log.debug(" no segments to download for client ",address ) ;
				break;
			}
			log.debug(" will download segments ", segmentsToDownload, " by client ",address) ;
			clientThreads.addNewThread(address, *this, [resource, segmentsToDownload](TinClientThread &clientThread ) {
				clientThread.startDownloadingProcess(resource, segmentsToDownload);
			});

		}
	}
}

void Kernel::removeRevertedResource(Resource &resource) {
	log.debug(Help::Str("Removing revertedResource ", resource));
	tinNetworkState.removeResource(resource);
	bool wasResourcePresent = localResourcesStateInfo.isLocalResourcePresent(resource);
	localResourcesStateInfo.setAsReverted(resource);
	if( wasResourcePresent){
		fileManagerThread->add([resource](FileManagerThread &f ){ f.removeResource(resource);});
	}
}

void Kernel::removeConnectionToClient(TinAddress &address) {
	tinNetworkState.removeClient(address);
	std::shared_ptr<TinClientThread> clientThread = clientThreads.get(address);
	auto requested = clientThread->getRequestedResource();
	if(requested.getResourceSize() != 0){ // was  not uninitialized
		localResourcesStateInfo.deallocateSegmentRange( clientThread->getRequestedResource(), clientThread->getRequestedSegments() );
	}
	clientThreads.removeThread(address);
}

void Kernel::killApplication() {
	log.info(" killing application." );

	std::exit(-1);
}

void Kernel::sig_handler(int sig, siginfo_t *info, void *context) {
	char buf[400];
	if( sig == 2 ){
		std::exit(-1);
	}

	if (sig == SIGILL || sig == SIGFPE || sig == SIGSEGV || sig == SIGBUS || sig == SIGCHLD || sig == SIGTRAP || sig == SIGPOLL)
		sprintf(buf, "Signal = %d, subcode = %d, pid = %d, uid = %d, address = %p, errno = %d\n", sig, info->si_code, info->si_pid, info->si_uid, info->si_addr, info->si_errno);
	else
		sprintf(buf, "Signal = %d, pid = %d, uid = %d, address = %p, errno = %d\n", sig, info->si_pid, info->si_uid, info->si_addr, info->si_errno);

	std::cerr << "HANDLED SIGNAL " << buf << std::endl;
}

void Kernel::setSignalHandler() {
	struct sigaction act;
	sigemptyset(&act.sa_mask); // or sigfillset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = sig_handler;

	sigaction(SIGABRT, &act, 0);
	sigaction(SIGALRM, &act, 0);
	sigaction(SIGFPE, &act, 0);
	sigaction(SIGHUP, &act, 0);
	sigaction(SIGILL, &act, 0);
	sigaction(SIGINT, &act, 0);
	sigaction(SIGPIPE, &act, 0);
}


