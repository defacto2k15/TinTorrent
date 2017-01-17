//
// Created by defacto on 21.12.16.
//

#include <fstream>
#include <Utils/StreamUtils.h>
#include <Common/InMemoryBuffer.h>
#include <Test/FileTestingBase.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "FileManager/FileManager.h"
#include "../../Assertions/Assertions.h"
#include "Common/SegmentsSet.h"


class FileMangerTest: public ::testing::Test, public FileTestingBase {
	virtual void SetUp() {
		fileSetUp();
	}
};

TEST_F(FileMangerTest, InitialFilesWithFullMetadataAndSegments){
	addFileToWorkingDirectory( L"file1.txt.9000", 9000 );
	addFileToWorkingDirectory( L"file1.txt.9000.metadata", {true, true, true});
	addFileToWorkingDirectory( L"file2.txt.7000", 7000);
	addFileToWorkingDirectory( L"file2.txt.7000.metadata", {true, true}  );

	FileManager manager(workingDirectory);
	auto result = manager.initialCheck();

	ASSERT_EQ( result.size(), 2u );
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file1.txt", 9000), FileSegmentsInfo( { true, true, true} ) ) ));
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file2.txt", 7000), FileSegmentsInfo( { true, true} ) ) ));
}

TEST_F(FileMangerTest, InitialFilesWithFullMetadataAndNotAllSegments){
	addFileToWorkingDirectory( L"file1.txt.9000", 9000 );
	addFileToWorkingDirectory( L"file1.txt.9000.metadata", {true, false, true});
	addFileToWorkingDirectory( L"file2.txt.7000", 7000);
	addFileToWorkingDirectory( L"file2.txt.7000.metadata", {false, true}  );

	FileManager manager(workingDirectory);
	auto result = manager.initialCheck();

	ASSERT_EQ( result.size(), 2u );
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file1.txt", 9000), FileSegmentsInfo( { true, false, true} ) ) ));
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file2.txt", 7000), FileSegmentsInfo( { false, true} ) ) ));
}

TEST_F(FileMangerTest, InitialFilesWithoutMetadataHaveSegmentsSetAsFull){
	addFileToWorkingDirectory( L"file1.txt.9000", 9000 );
	addFileToWorkingDirectory( L"file2.txt.7000", 7000);

	FileManager manager(workingDirectory);
	auto result = manager.initialCheck();

	ASSERT_EQ( result.size(), 2u );
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file1.txt", 9000), FileSegmentsInfo( { true, true, true} ) ) ));
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file2.txt", 7000), FileSegmentsInfo( { true, true} ) ) ));
	ASSERT_TRUE( fileExists(L"file1.txt.9000.metadata"));
	ASSERT_TRUE( fileExists(L"file2.txt.7000.metadata"));
}

TEST_F(FileMangerTest, MetadataFilesWithoutResourceAreDeleted){
	addFileToWorkingDirectory( L"file1.txt", 9000 );
	addFileToWorkingDirectory( L"file2.txt", 7000);
	addFileToWorkingDirectory( L"fileX.txt.9999.metadata", {false, true, false});

	FileManager manager(workingDirectory);
	manager.initialCheck();

	ASSERT_FALSE( fileExists(L"fileX.txt.9999.metadata"));
}
//TODO: what if metadata size in name is wrong (doesn't match)

TEST_F(FileMangerTest, ResourceWithoutSizeInNameIsGivenSize){
	addFileToWorkingDirectory( L"file1.txt", 9000 );

	FileManager manager(workingDirectory);
	auto result = manager.initialCheck();
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file1.txt", 9000), FileSegmentsInfo( { true, true, true} ) ) ));

	ASSERT_TRUE( fileExists(L"file1.txt.9000.metadata"));
	ASSERT_FALSE( fileExists(L"file1.txt"));
}


TEST_F(FileMangerTest, TwoBytesSegmentDataWorks){
	addFileToWorkingDirectory( L"file1.txt.40960", 40960);
	std::vector<bool> segmentPresence(10, true);
	addFileToWorkingDirectory( L"file1.txt.40960.metadata", segmentPresence);

	FileManager manager(workingDirectory);

	auto result = manager.initialCheck();
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file1.txt", 40960), FileSegmentsInfo( segmentPresence ) )));
}

TEST_F(FileMangerTest, UpdateSeesNewFileCreated){
	addFileToWorkingDirectory( L"file1.txt", 9000 );
	addFileToWorkingDirectory( L"file2.txt", 7000 );

	FileManager manager(workingDirectory);
	manager.initialCheck();
	addFileToWorkingDirectory( L"file3.txt.10000", 10000 );
	addFileToWorkingDirectory( L"file3.txt.10000.metadata", {false,  true, false});
	auto updateInfo = manager.update();

	ASSERT_THAT( updateInfo.getNewFiles(), ::testing::Contains(
		FileInfo( Resource(L"file3.txt", 10000), FileSegmentsInfo( { false, true, false} ) ) )
	);
}

TEST_F(FileMangerTest, UpdateSeesFileRemoved){
	addFileToWorkingDirectory( L"file1.txt", 9000 );
	addFileToWorkingDirectory( L"file2.txt", 7000 );

	FileManager manager(workingDirectory);
	manager.initialCheck();
	removeFile(L"file1.txt.9000");
	auto updateInfo = manager.update();

	ASSERT_THAT( updateInfo.getDeletedFiles(), ::testing::Contains(
			FileInfo( Resource(L"file1.txt", 9000), FileSegmentsInfo( { true, true, true} ) ) )
	);
}

TEST_F(FileMangerTest, AskingForSegmentWorks){
	InMemoryBuffer fileContents(9000);
	fileContents.setSize(9000);
	setBufferContentToIndex( fileContents);

	addFileToWorkingDirectory( L"file1.txt", fileContents);

	FileManager manager(workingDirectory);
	manager.initialCheck();

	SegmentsSet outSet = manager.getSegments( Resource(L"file1.txt", 9000), SegmentRange(0, 3));

	ASSERT_EQ( outSet.getRange(), SegmentRange(0,3));
	ASSERT_EQ( outSet.getSegment(0).getPayload(), fileContents.getSubset(0, Constants::segmentSize) );
	ASSERT_EQ( outSet.getSegment(0).getSegmentInfo(), SegmentInfo(0, Constants::segmentSize) );
	ASSERT_EQ( outSet.getSegment(1).getPayload(), fileContents.getSubset( Constants::segmentSize, Constants::segmentSize*2  ) );
	ASSERT_EQ( outSet.getSegment(1).getSegmentInfo(), SegmentInfo(1, Constants::segmentSize) );
	ASSERT_EQ( outSet.getSegment(2).getPayload(), fileContents.getSubset( Constants::segmentSize*2, 9000 ) );
	ASSERT_EQ( outSet.getSegment(2).getSegmentInfo(), SegmentInfo(2, 9000 - Constants::segmentSize*2) );
}

TEST_F(FileMangerTest, SavingSegmentWorks){
	InMemoryBuffer fileContents(9000);
	fileContents.setSize(9000);
	setBufferContentToIndex( fileContents);

	auto segment1 = std::make_shared<InMemoryBuffer>(Constants::segmentSize);
	setBufferToConstant(*segment1, 4);

	auto segment2 = std::make_shared<InMemoryBuffer>(9000u - Constants::segmentSize*2);
	setBufferToConstant(*segment2, 8);

	addFileToWorkingDirectory( L"file1.txt", fileContents);

	FileManager manager(workingDirectory);
	manager.initialCheck();
	manager.setSegments( Resource(L"file1.txt", 9000),
	                     SegmentsSet(SegmentRange(1,3),
	                                 {Segment(segment1, SegmentInfo(1, Constants::segmentSize)),
	                                  Segment(segment2, SegmentInfo(2, uint16_t( 9000u - Constants::segmentSize*2) ))}));

	fileContents.setData(*segment1, Constants::segmentSize);
	fileContents.setData(*segment2, (size_t)Constants::segmentSize*2);

	InMemoryBuffer actualFileContents(9000);
	FileUtils::readContent(getPathToFile(L"file1.txt.9000"), actualFileContents, 0, 9000 );
	ASSERT_EQ( fileContents, actualFileContents);
}

TEST_F(FileMangerTest, FileRemovalIsWorking){
	addFileToWorkingDirectory( L"file1.txt", 9000 );
	addFileToWorkingDirectory( L"file2.txt", 7000 );

	FileManager manager(workingDirectory);
	manager.initialCheck();

	ASSERT_TRUE(fileExists(L"file1.txt.9000"));
	ASSERT_TRUE(fileExists(L"file1.txt.9000.metadata"));
	Resource resource(L"file1.txt", 9000);
	manager.removeResource( resource );

	ASSERT_FALSE(fileExists(L"file1.txt.9000"));
	ASSERT_FALSE(fileExists(L"file1.txt.9000.metadata"));
}

TEST_F(FileMangerTest, FileCreationIsWorking){
	addFileToWorkingDirectory( L"file1.txt", 9000 );
	addFileToWorkingDirectory( L"file2.txt", 7000 );

	FileManager manager(workingDirectory);
	manager.initialCheck();

	ASSERT_FALSE(fileExists(L"file3.txt.11000"));
	ASSERT_FALSE(fileExists(L"file3.txt.11000.metadata"));

	Resource resource(L"file3.txt", 11000);
	manager.createEmptyResource( resource );

	ASSERT_TRUE(fileExists(L"file3.txt.11000"));
	ASSERT_TRUE(fileExists(L"file3.txt.11000.metadata"));
}


// todo renaming problem: there is file1.txt.7000 and we  are adding file1.txt which has 7000 bytes. should be illegal!
