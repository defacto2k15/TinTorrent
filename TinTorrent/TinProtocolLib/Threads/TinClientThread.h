//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_TINCLIENTTHREAD_H
#define TINTORRENT_TINCLIENTTHREAD_H


#include <Multithreading/ActionQueue.h>
#include <TinProtocolLib/TinClientSocket.h>
#include <Common/SegmentRange.h>

class Kernel;

class TinClientThread : public ActionQueue<TinClientThread>{
	TinClientSocket clientSocket;
	TinAddress addressToConnect;
	Kernel &kernel;
	std::unique_ptr<TinConnectedClientSocket> connectedSocket;
	Resource requestedResource;
	SegmentRange requestedSegments;
	bool isConnectionOpen = false;
public:
	TinClientThread( TinAddress &addressToConnect, Kernel &kernel );

	void startDownloadingProcess(Resource resource, SegmentRange segmentsToDownload);

	void sendResourceRequest(  );

	void recieveSegments( SegmentRange segmentRange, MessageStartSendingRequest::PreviousStatus prevStatus);

	void closeConnection( MessageClose::CloseReason closeReason );

	Resource getRequestedResource() const;

	SegmentRange getRequestedSegments() const;

	void genericCloseConnection();

	bool hasOpenedConnection();

private:
	void handleException( std::function<void()> func );
};


#endif //TINTORRENT_TINCLIENTTHREAD_H
