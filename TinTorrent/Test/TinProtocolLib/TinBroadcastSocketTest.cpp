//
// Created by defacto on 31.12.16.
//

#include <gtest/gtest.h>
#include <Multithreading/MultithreadingSequence.h>
#include <Test/ThreadInTest.h>
#include <TinProtocolLib/TinBroadcastSocket.h>
#include <TinProtocolLib/TinListeningUdpSocket.h>

class TinBroadcastSocketTest : public ::testing::Test {
public:
	MultithreadingSequence sequence;
	std::string currentSequenceName;

};

TEST_F(TinBroadcastSocketTest, TwoSocketsCanTalkToThemselves) {
	currentSequenceName = test_info_->name();
	sequence.initBlockingElement(currentSequenceName);
	TinAddress destination("127.0.0.0", Constants::broadcastPort);
	std::vector<Resource> announcedResource{ Resource(L"file.txt", 21321), Resource(L"file2.txt", 6421), Resource(L"file3.txt", 1123)};

	ThreadInTest<bool> clientThread([this, destination, &announcedResource](){
		TinBroadcastSocket serverSocket(destination);
		serverSocket.initSocket();
		sequence[currentSequenceName]->waitForUnlock();
		serverSocket.sendAnnounceMessage(announcedResource);
		return true;
	});
	auto clientFuture = clientThread.run();

	ThreadInTest<BroadcastMessage> serverThread( [this](){
		TinListeningUdpSocket clientSocket;
		clientSocket.initSocket();
		sequence[currentSequenceName]->unlockAfter(1000);
		return clientSocket.listenForMessages().first;
	});
	auto serverFuture = serverThread.run();

	EXPECT_EQ( serverFuture.get().getResources(), announcedResource);
}