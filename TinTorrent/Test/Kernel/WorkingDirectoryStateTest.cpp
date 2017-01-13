//
// Created by defacto on 13.01.17.
//
#include <gtest/gtest.h>
#include <Kernel/WorkingDirectoryState.h>
#include "WorkingDirectoryStateTest.h"

class WorkingDirectoryStateTest : public ::testing::Test {
public:
	WorkingDirectoryState state;

	std::vector<FileInfo> initialFiles{
			FileInfo(Resource(L"file1.txt",10000), FileSegmentsInfo({true, true, true})),
			FileInfo(Resource(L"file2.txt",15000), FileSegmentsInfo({true, true, false, false})),
	};

};

TEST_F( WorkingDirectoryStateTest, IsDownloadedWorksOk) {
	state.init(initialFiles);
	ASSERT_TRUE( state.isDownloaded(initialFiles[0].getResource()));
	ASSERT_FALSE( state.isDownloaded(initialFiles[1].getResource()));
}

TEST_F( WorkingDirectoryStateTest, ContainsWorks) {
	ASSERT_FALSE( state.contains(initialFiles[0].getResource()));
	ASSERT_FALSE( state.contains(initialFiles[0].getResource()));

	state.init(initialFiles);

	ASSERT_TRUE( state.contains(initialFiles[0].getResource()));
	ASSERT_TRUE( state.contains(initialFiles[0].getResource()));
}


TEST_F( WorkingDirectoryStateTest, AddResourceWorks) {
	Resource newResource(L"FFFile13.txt", 4096*4 + 100);

	state.init(initialFiles);

	state.addResource(newResource);
	ASSERT_TRUE( state.contains(newResource));
	ASSERT_TRUE( state.isDownloaded(newResource));
}


TEST_F( WorkingDirectoryStateTest, AddEmptyResourceWorks) {
	Resource newResource(L"FFFile13.txt", 4096*4 + 100);

	state.init(initialFiles);

	state.addEmptyResource(newResource);
	ASSERT_TRUE( state.contains(newResource));
	ASSERT_FALSE( state.isDownloaded(newResource));
}

TEST_F( WorkingDirectoryStateTest, SingleAllocateWithEmptyFileWorks) {
	Resource newResource(L"FFFile13.txt", 4096*4 + 100);

	state.init(initialFiles);

	state.addEmptyResource(newResource);
	auto segmentRange = state.allocateSegmentsToDownload(newResource);

	ASSERT_EQ(segmentRange.getMin(), 0);
	ASSERT_EQ(segmentRange.getMax(), Constants::maxSegmentChunkSize);
}

TEST_F( WorkingDirectoryStateTest, SecondAllocateWithEmptyFileWorks) {
	Resource newResource(L"FFFile13.txt", 4096*4 + 100);

	state.init(initialFiles);

	state.addEmptyResource(newResource);
	state.allocateSegmentsToDownload(newResource);
	auto segmentRange = state.allocateSegmentsToDownload(newResource);

	ASSERT_EQ(segmentRange.getMin(), Constants::maxSegmentChunkSize);
	ASSERT_EQ(segmentRange.getMax(), 4);
}

TEST_F( WorkingDirectoryStateTest, AllocateWhenThereIsNoMoreToAllocateWorks) {
	Resource newResource(L"FFFile13.txt", 4096*2 + 100);

	state.init(initialFiles);

	state.addEmptyResource(newResource);
	state.allocateSegmentsToDownload(newResource);
	state.allocateSegmentsToDownload(newResource);

	auto segmentRange = state.allocateSegmentsToDownload(newResource);
	ASSERT_TRUE(segmentRange.empty());
}

TEST_F( WorkingDirectoryStateTest, DealocateSegmentWorks) {
	Resource newResource(L"FFFile13.txt", 4096*4 + 100);

	state.init(initialFiles);

	state.addEmptyResource(newResource);
	state.allocateSegmentsToDownload(newResource);
	auto middleRange = state.allocateSegmentsToDownload(newResource);
	state.allocateSegmentsToDownload(newResource);

	auto segmentRange = state.allocateSegmentsToDownload(newResource);
	ASSERT_TRUE(segmentRange.empty());

	state.deallocateSegmentRange(newResource, middleRange);
	ASSERT_EQ( middleRange, state.allocateSegmentsToDownload(newResource));
}