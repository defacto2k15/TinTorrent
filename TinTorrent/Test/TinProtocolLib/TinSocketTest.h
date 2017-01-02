//
// Created by defacto on 09.12.16.
//

#ifndef TINTORRENT_TINSOCKETTEST_H
#define TINTORRENT_TINSOCKETTEST_H
#include "gtest/gtest.h"
#include "../../TinProtocolLib/TinServerSocket.h"
#include "../../Common/ExHelp.h"
#include "../../TinProtocolLib/TinConnectedClientSocket.h"
#include "../../TinProtocolLib/TinClientSocket.h"
#include "../../Multithreading/MultithreadingSequence.h"
#include "../../TinProtocolLib/MessageCloseException.h"
#include <thread>
#include <future>
#include <Test/ThreadInTest.h>

static bool testWasInitialized = false;

class TinSocketTest : public ::testing::Test {
public:
	MultithreadingSequence sequence;
	std::string currentSequenceName;
	uint16_t portNumber;

	TinSocketTest() : ::testing::Test(){
		if( !testWasInitialized ){
			srand (time(NULL));
			testWasInitialized = true;
		}
	}

	virtual void SetUp() {
		portNumber = getRandomPortNumber();
	}


	uint16_t getRandomPortNumber(){
		return rand() % 50000 + 10000;
	}

	template<typename T>
	T doServerAction( std::function<T(TinConnectedServerSocket &connectedServerSocket)> action){
		TinServerSocket serverSocket(portNumber);
		serverSocket.initSocket();
		sequence[currentSequenceName]->unlockAfter(400);
		auto connectedSocket = serverSocket.listenForConnections();

		T result = action(connectedSocket);

		connectedSocket.closeSocket();
		serverSocket.closeSocket();
		return result;
	}

	template<typename T>
	T doClientAction( std::function<T(TinConnectedClientSocket &connectedClientSocket)> action){
		TinClientSocket clientSocket(TinAddress("127.0.0.1", portNumber));
		clientSocket.initSocket();
		sequence[currentSequenceName]->waitForUnlock();
		auto connectedSocket = clientSocket.connect();
		T result = action(connectedSocket);
		connectedSocket.closeSocket();
		clientSocket.closeSocket();
		return result;
	}

};



TEST_F(TinSocketTest, SocketsCanGetConnected) {
	currentSequenceName = test_info_->name();
	sequence.initBlockingElement(currentSequenceName);

	ThreadInTest<bool> serverThread ([this](){
		TinServerSocket serverSocket(portNumber);
		serverSocket.initSocket();
		sequence[currentSequenceName]->unlockAfter(400);
		auto connectedSocket = serverSocket.listenForConnections();
		connectedSocket.closeSocket();
		serverSocket.closeSocket();
		return true;
	} );
	auto serverFuture = serverThread.run();

	ThreadInTest<bool> clientThread( [this](){
		TinClientSocket clientSocket(TinAddress("127.0.0.1", portNumber));
		clientSocket.initSocket();
		sequence[currentSequenceName]->waitForUnlock();
		auto connectedSocket = clientSocket.connect();
		connectedSocket.closeSocket();
		clientSocket.closeSocket();
		return true;
	});
	auto clientFuture =  clientThread.run();

	EXPECT_TRUE(serverFuture.get()) << "Server future";
	EXPECT_TRUE(clientFuture.get()) << "Client future";
}

TEST_F(TinSocketTest, ResourceRequestRecievingWorks) {
	currentSequenceName = test_info_->name();
	sequence.initBlockingElement(currentSequenceName);
	Resource resourceToAskFor(L"file.txt", 4444);

	ThreadInTest<Resource> serverThread ([this](){
		return doServerAction<Resource>([](TinConnectedServerSocket &serverSocket){
			Resource resource = serverSocket.listenForResourceRequest();
			return resource;
		});
	} );
	auto serverFuture = serverThread.run();

	ThreadInTest<bool> clientThread( [this, resourceToAskFor](){
		return doClientAction<bool>([resourceToAskFor](TinConnectedClientSocket &clientSocket){
			clientSocket.sendResourceRequest(resourceToAskFor);
			return true;
		});
	});
	auto clientFuture =  clientThread.run();

	EXPECT_TRUE(clientFuture.get()) << "Client future";
	EXPECT_EQ(serverFuture.get(), resourceToAskFor);
}


TEST_F(TinSocketTest, ResourceResponseWorks) {
	currentSequenceName = test_info_->name();
	sequence.initBlockingElement(currentSequenceName);
	auto responseValue = MessageResourceResponse::ResourceResponseValue::OK;

	ThreadInTest<bool> serverThread ([this, responseValue](){
		return doServerAction<bool>([responseValue](TinConnectedServerSocket &serverSocket){
			serverSocket.listenForResourceRequest();
			serverSocket.sendResourceResponse(responseValue);
			return true;
		});
	} );
	auto serverFuture = serverThread.run();

	ThreadInTest<MessageResourceResponse::ResourceResponseValue> clientThread( [this](){
		return  doClientAction<MessageResourceResponse::ResourceResponseValue>([](TinConnectedClientSocket &clientSocket){
			clientSocket.sendResourceRequest(Resource(L"file.txt", 4444));
			return clientSocket.listenForResourceResponse();
		});
	});
	auto clientFuture =  clientThread.run();

	EXPECT_TRUE(serverFuture.get()) << "Server future";
	EXPECT_EQ(clientFuture.get(), responseValue);
}


TEST_F(TinSocketTest, StartSendingRequestWorks) {
	currentSequenceName = test_info_->name();
	sequence.initBlockingElement(currentSequenceName);
	MessageStartSendingRequest expectedStartSendingRequest
			(SegmentInfo( 3, 1231), MessageStartSendingRequest::PreviousStatus::OK);

	ThreadInTest<MessageStartSendingRequest> serverThread ([this](){
		return doServerAction<MessageStartSendingRequest>([](TinConnectedServerSocket &serverSocket){
			serverSocket.listenForResourceRequest();
			serverSocket.sendResourceResponse(MessageResourceResponse::ResourceResponseValue::OK);
			return serverSocket.listenForStartSendingRequest();
		});
	} );
	auto serverFuture = serverThread.run();

	ThreadInTest<bool> clientThread( [this, &expectedStartSendingRequest](){
		return  doClientAction<bool>([&expectedStartSendingRequest](TinConnectedClientSocket &clientSocket){
			clientSocket.sendResourceRequest(Resource(L"file.txt", 4444));
			clientSocket.listenForResourceResponse();
			clientSocket.sendStartSendingRequest(expectedStartSendingRequest.getSegmentInfo(), expectedStartSendingRequest.getPreviousStatus());
			return true;
		});
	});
	auto clientFuture =  clientThread.run();

	EXPECT_TRUE(clientFuture.get()) << "client future";
	EXPECT_EQ(expectedStartSendingRequest, serverFuture.get());
}

TEST_F(TinSocketTest, SendSegmentResponseWorks) {
	currentSequenceName = test_info_->name();
	sequence.initBlockingElement(currentSequenceName);
	uint8_t segmentPayload[4000];
	std::fill(segmentPayload, segmentPayload+4000, 0xbe);
	SegmentInfo segmentInfo(1, 4000);

	ThreadInTest<bool> serverThread ([&segmentInfo, &segmentPayload, this](){
		return doServerAction<bool>([&segmentInfo, &segmentPayload, this](TinConnectedServerSocket &serverSocket){
			serverSocket.listenForResourceRequest();
			serverSocket.sendResourceResponse(MessageResourceResponse::ResourceResponseValue::OK);
			serverSocket.listenForStartSendingRequest();
			serverSocket.sendSegmentResponse(segmentInfo, segmentPayload);
			return true;
		});
	} );
	auto serverFuture = serverThread.run();

	uint8_t outBuffer[5000];
	ThreadInTest<bool> clientThread( [this, &segmentInfo, &outBuffer](){
		return  doClientAction<bool>([&segmentInfo, &outBuffer](TinConnectedClientSocket &clientSocket){
			clientSocket.sendResourceRequest(Resource(L"file.txt", 4444));
			clientSocket.listenForResourceResponse();
			clientSocket.sendStartSendingRequest(segmentInfo, MessageStartSendingRequest::PreviousStatus::OK);
			SegmentResponse *response = clientSocket.listenForSegmentResponse();
			memcpy(outBuffer, response, response->getWholeResponseSize());
			return true;
		});
	});
	auto clientFuture =  clientThread.run();
	SegmentResponse *response = (SegmentResponse*)outBuffer;
	EXPECT_TRUE(serverFuture.get()) << "Server future";
	EXPECT_TRUE(clientFuture.get()) << "Client future";
	EXPECT_EQ(response->constantId[0], 0xff);
	EXPECT_EQ(response->constantId[1], 0xff);
	EXPECT_EQ(response->payloadLength, segmentInfo.getPayloadLength());
	EXPECT_EQ(response->segmentIndex, segmentInfo.getSegmentIndex());
	EXPECT_TRUE( 0 == memcmp(segmentPayload, response->payload, response->payloadLength));
}

TEST_F(TinSocketTest, CloseWhileWaitingForSegmentRequestIsSignalizedByException) {
	currentSequenceName = test_info_->name();
	sequence.initBlockingElement(currentSequenceName);
	uint8_t segmentPayload[4000];
	std::fill(segmentPayload, segmentPayload+4000, 0xbe);
	SegmentInfo segmentInfo(1, 4000);

	ThreadInTest<bool> serverThread ([&segmentInfo, &segmentPayload, this](){
		return doServerAction<bool>([&segmentInfo, &segmentPayload, this](TinConnectedServerSocket &serverSocket){
			serverSocket.listenForResourceRequest();
			serverSocket.sendResourceResponse(MessageResourceResponse::ResourceResponseValue::OK);
			serverSocket.listenForStartSendingRequest();
			serverSocket.sendSegmentResponse(SegmentInfo(1,4000), segmentPayload);
			serverSocket.listenForStartSendingRequest();
			serverSocket.closeConnection(MessageClose::CloseReason::OK);
			serverSocket.closeSocket();
			return true;
		});
	} );
	auto serverFuture = serverThread.run();

	ThreadInTest<MessageCloseException> clientThread( [this](){
		return  doClientAction<MessageCloseException>([](TinConnectedClientSocket &clientSocket){
			clientSocket.sendResourceRequest(Resource(L"file.txt", 4444));
			clientSocket.listenForResourceResponse();
			clientSocket.sendStartSendingRequest(SegmentInfo(1,4000), MessageStartSendingRequest::PreviousStatus::OK);
			clientSocket.listenForSegmentResponse();
			clientSocket.sendStartSendingRequest(SegmentInfo(2,2000), MessageStartSendingRequest::PreviousStatus::OK);
			try {
				clientSocket.listenForSegmentResponse();
			} catch( MessageCloseException ex){
				return ex;
			}
			throw std::logic_error("Nie wyrzucony wyjatem MessageCloseException");
		});
	});
	auto clientFuture = clientThread.run();

	EXPECT_TRUE(serverFuture.get()) << "server future";
	MessageCloseException ex = clientFuture.get();
	EXPECT_EQ(ex.getMessageClose(), MessageClose(MessageClose::CloseReason::OK));
}

#endif //TINTORRENT_TINSOCKETTEST_H
