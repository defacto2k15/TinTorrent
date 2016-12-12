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
#include <thread>
#include <future>

class TinSocketTest : public ::testing::Test {
public:
	MultithreadingSequence sequence;
	std::string currentSequenceName;
	uint16_t portNumber;

	TinSocketTest() : ::testing::Test(){
		srand (time(NULL));
	}

	virtual void SetUp() {
		portNumber = getRandomPortNumber();
	}


	uint16_t getRandomPortNumber(){
		return rand() % 50000 + 10000;
	}


};
template<typename T>
class ThreadInTest{
	std::function<T()> funcToRun;
	std::shared_ptr<std::thread> testThread;
public:
	ThreadInTest(std::function<T()> funcToRun ) : funcToRun(funcToRun){
	}

	std::future<T> run(){
		std::promise< T> testPromise;
		auto testFuture = testPromise.get_future();
		testThread = std::make_shared<std::thread>(std::bind([ this](std::promise<T> &testPromise){
			try {
				testPromise.set_value( funcToRun());
			} catch( Assertions::AssertionException &e){
				ExHelp::writeException(e, "serverThread");
				testPromise.set_exception(std::current_exception());
			} catch( ...){
				std::cerr<<" Unexpected exception caught " << std::endl;
				testPromise.set_exception(std::current_exception());
			}
		}, std::move(testPromise)));
		return testFuture;
	}

	~ThreadInTest(){
		testThread->join();
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

TEST_F(TinSocketTest, ResourceRequestCanBeHeader) {
	currentSequenceName = test_info_->name();
	sequence.initBlockingElement(currentSequenceName);
	Resource resourceToAskFor(L"file.txt", 4444);

	ThreadInTest<Resource> serverThread ([this](){
		TinServerSocket serverSocket(portNumber);
		serverSocket.initSocket();
		sequence[currentSequenceName]->unlockAfter(400);
		auto connectedSocket = serverSocket.listenForConnections();

		Resource resource = connectedSocket.listenForResourceRequest();
		connectedSocket.closeSocket();
		serverSocket.closeSocket();
		return resource;
	} );
	auto serverFuture = serverThread.run();

	ThreadInTest<bool> clientThread( [this, resourceToAskFor](){
		TinClientSocket clientSocket(TinAddress("127.0.0.1", portNumber));
		clientSocket.initSocket();
		sequence[currentSequenceName]->waitForUnlock();
		auto connectedSocket = clientSocket.connect();
		connectedSocket.sendResourceRequest(resourceToAskFor);
		connectedSocket.closeSocket();
		clientSocket.closeSocket();
		return true;
	});
	auto clientFuture =  clientThread.run();

	EXPECT_TRUE(clientFuture.get()) << "Client future";
	EXPECT_EQ(serverFuture.get(), resourceToAskFor);
}


#endif //TINTORRENT_TINSOCKETTEST_H
