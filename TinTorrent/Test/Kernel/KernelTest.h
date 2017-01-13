//
// Created by defacto on 02.01.17.
//

#ifndef TINTORRENT_KERNELTEST_H
#define TINTORRENT_KERNELTEST_H

#include <gtest/gtest.h>
#include <Kernel/Kernel.h>
#include <Test/ThreadInTest.h>
#include <Multithreading/MultithreadingSequence.h>
#include <Test/FileTestingBase.h>


using namespace std::chrono_literals;

class KernelTest : public ::testing::Test, public FileTestingBase {
public:
	MultithreadingSequence sequence;
	std::string currentSequenceName;


	virtual void SetUp() {
		fileSetUp();
	}
};

TEST_F( KernelTest, FileInWorkingDirectoryAtStartAreAccesible){
	std::vector< Resource > resourcesToTest{
			Resource(L"file1.txt",123), Resource(L"file2.txt", 223), Resource(L"file3.txt", 999)
	};
	for( auto &resource : resourcesToTest){
		addFileToWorkingDirectory( resource.getResourceName(), resource.getResourceSize());
	}

	Kernel kernel;
	kernel.startApp( workingDirectory);
	std::this_thread::sleep_for( 2s);
	auto localResources = kernel.getProgramInfoProvider().getLocalResources();
	kernel.closeKernel();

	EXPECT_EQ( localResources.size(), resourcesToTest.size() ) << "Resources count";
	for( auto &localResource : localResources){
		EXPECT_TRUE( ContainerUtils::Contains(resourcesToTest, localResource.resource));
		EXPECT_EQ( 100, localResource.percentDownloaded);
	}
}

TEST_F( KernelTest, BroadcastedResourcesAreBeingNoticed){
	currentSequenceName = test_info_->name();
	sequence.initBlockingElement(currentSequenceName);
	sequence.initBlockingElement(currentSequenceName+"2");
	std::vector< Resource > resourcesToAnnounce{
			Resource(L"file1.txt",123), Resource(L"file2.txt", 223), Resource(L"file3.txt", 999)
	};

	ThreadInTest<bool> otherBroadcastingThread1( [&](){
		TinBroadcastSocket socket(TinAddress("127.0.0.1", Constants::broadcastPort) );
		socket.initSocket();
		sequence[currentSequenceName]->waitForUnlock();
		socket.sendAnnounceMessage( resourcesToAnnounce);
		sequence[currentSequenceName+"2"]->unlockAfter(500);
		return true;
	});
	auto otherFuture = otherBroadcastingThread1.run();

	Kernel kernel;
	kernel.startApp( workingDirectory);
	std::this_thread::sleep_for(3s);
	sequence[currentSequenceName]->unlock();
	sequence[currentSequenceName+"2"]->waitForUnlock();
	auto resourcesInOtherClients = kernel.getProgramInfoProvider().getResourcesInOtherClients();
	auto resourcesToDownload = kernel.getProgramInfoProvider().getResourcesToDownload();
	kernel.closeKernel();

	EXPECT_TRUE( otherFuture.get()) << "Server future";

	EXPECT_EQ( resourcesInOtherClients.size(), 1 ) << "Resources count";
	auto resourcesInOnlyClient = resourcesInOtherClients[0];

	for( auto &expectedResource : resourcesToAnnounce){
		std::vector<Resource> actualResources = resourcesInOnlyClient.getResources();
		EXPECT_TRUE( ContainerUtils::Contains(actualResources, expectedResource));
		EXPECT_TRUE( ContainerUtils::Contains(resourcesToDownload, expectedResource));
	}
}

#endif //TINTORRENT_KERNELTEST_H
