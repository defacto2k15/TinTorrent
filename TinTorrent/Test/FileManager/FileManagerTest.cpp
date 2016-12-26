//
// Created by defacto on 21.12.16.
//

#include <fstream>
#include <Utils/StreamUtils.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../../FileManager/FileManager.h"
#include "../Mocks/MockFileInformationConsumer.h"
#include "../../Assertions/Assertions.h"


#define TEST_WORKING_DIRECTORY L"/home/defacto/ClionProjects/TinTorrent/Test/TestWorkingDirectory"

#ifndef TEST_WORKING_DIRECTORY
#error "TEST_WORKING_DIRECTORY MUST BE DEFINED"
#endif

class FileMangerTest: public ::testing::Test {
	virtual void SetUp() {
		Assertions::check(workingDirectory.size() > 10, "Saved ur ass, faggot");
		system(("exec rm -r "+workingDirectory+"/*").c_str());
	}

public:
	std::string workingDirectory = StringHelp::toUtf8(std::wstring( TEST_WORKING_DIRECTORY));

	void addFileToWorkingDirectory( std::wstring fileName, size_t size){
		std::ofstream ofs( workingDirectory+std::string("/") + StringHelp::toUtf8(fileName), std::ios::binary | std::ios::out);
		ofs.seekp( size-1);
		ofs.write("", 1);
	}

	void addFileToWorkingDirectory( std::wstring fileName, std::vector<bool> payload){
		std::ofstream ofs( workingDirectory+std::string("/") + StringHelp::toUtf8(fileName), std::ios::binary | std::ios::out);
		StreamUtils::writeToStream(ofs, payload);
	}

	bool fileExists( std::wstring fileName ){
		std::ifstream infile(workingDirectory+std::string("/")+StringHelp::toUtf8(fileName));
		return infile.good();
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
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file1.txt.9000", 9000), FileSegmentsInfo( { true, true, true} ) ) ));
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file2.txt.7000", 7000), FileSegmentsInfo( { true, true} ) ) ));
}

TEST_F(FileMangerTest, InitialFilesWithFullMetadataAndNotAllSegments){
	addFileToWorkingDirectory( L"file1.txt.9000", 9000 );
	addFileToWorkingDirectory( L"file1.txt.9000.metadata", {true, false, true});
	addFileToWorkingDirectory( L"file2.txt.7000", 7000);
	addFileToWorkingDirectory( L"file2.txt.7000.metadata", {false, true}  );

	FileManager manager(workingDirectory);
	auto result = manager.initialCheck();

	ASSERT_EQ( result.size(), 2u );
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file1.txt.9000", 9000), FileSegmentsInfo( { true, false, true} ) ) ));
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file2.txt.7000", 7000), FileSegmentsInfo( { false, true} ) ) ));
}

TEST_F(FileMangerTest, InitialFilesWithoutMetadataHaveSegmentsSetAsFull){
	addFileToWorkingDirectory( L"file1.txt.9000", 9000 );
	addFileToWorkingDirectory( L"file2.txt.7000", 7000);

	FileManager manager(workingDirectory);
	auto result = manager.initialCheck();

	ASSERT_EQ( result.size(), 2u );
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file1.txt.9000", 9000), FileSegmentsInfo( { true, true, true} ) ) ));
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file2.txt.7000", 7000), FileSegmentsInfo( { true, true} ) ) ));
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
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file1.txt.9000", 9000), FileSegmentsInfo( { true, true, true} ) ) ));

	ASSERT_TRUE( fileExists(L"file1.txt.9000.metadata"));
	ASSERT_FALSE( fileExists(L"file1.txt"));
}


TEST_F(FileMangerTest, TwoBytesSegmentDataWorks){
	addFileToWorkingDirectory( L"file1.txt.40960", 40960);
	std::vector<bool> segmentPresence(10, true);
	addFileToWorkingDirectory( L"file1.txt.40960.metadata", segmentPresence);

	FileManager manager(workingDirectory);

	auto result = manager.initialCheck();
	ASSERT_THAT( result, ::testing::Contains(   FileInfo( Resource(L"file1.txt.40960", 40960), FileSegmentsInfo( segmentPresence ) )));
}

//TEST_F(FileMangerTest, UpdateSeesNewFileCreated){
//	addFileToWorkingDirectory( L"file1.txt", 9000 );
//	addFileToWorkingDirectory( L"file2.txt", 7000 );
//
//	FileManager manager(workingDirectory);
//	manager.initialCheck();
//	addFileToWorkingDirectory( L"file3.txt.10000", 10000 );
//	addFileToWorkingDirectory( L"file3.txt.10000.metadata", {false, true, false});
//	auto updateInfo = manager.update();
//
//	ASSERT_THAT( updateInfo.getNewResources(), ::testing::Contains(
//		FileInfo( Resource(L"file3.txt.10000", 10000), FileSegmentsInfo( { false, true, false} ) ) )
//	);
//}


