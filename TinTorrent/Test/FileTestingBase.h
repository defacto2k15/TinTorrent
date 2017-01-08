//
// Created by defacto on 08.01.17.
//

#ifndef TINTORRENT_FILETESTINGBASE_H
#define TINTORRENT_FILETESTINGBASE_H

// todo
#include <Common/StringHelp.h>
#include <fstream>
#include <Common/Buffer.h>
#include <Utils/StreamUtils.h>
#include <Utils/FileUtils.h>

#define TEST_WORKING_DIRECTORY L"/home/defacto/ClionProjects/TinTorrent/Test/TestWorkingDirectory"

#ifndef TEST_WORKING_DIRECTORY
#error "TEST_WORKING_DIRECTORY MUST BE DEFINED"
#endif



class FileTestingBase {
public:
	void fileSetUp() {
		Assertions::check(workingDirectory.size() > 10, "Saved ur ass, faggot");
		system(("exec rm -r " + workingDirectory + "/*").c_str());
	}

	std::string workingDirectory = StringHelp::toUtf8(std::wstring(TEST_WORKING_DIRECTORY));

	void addFileToWorkingDirectory(std::wstring fileName, size_t size) {
		std::ofstream ofs(workingDirectory + std::string("/") + StringHelp::toUtf8(fileName),
		                  std::ios::binary | std::ios::out);
		ofs.seekp(size - 1);
		ofs.write("", 1);
	}

	void addFileToWorkingDirectory(std::wstring fileName, std::vector<bool> payload) {
		std::ofstream ofs(workingDirectory + std::string("/") + StringHelp::toUtf8(fileName),
		                  std::ios::binary | std::ios::out);
		StreamUtils::writeToStream(ofs, payload);
	}

	void addFileToWorkingDirectory(std::wstring fileName, Buffer &payload) {
		std::ofstream ofs(getPathToFile(fileName), std::ios::binary | std::ios::out);
		StreamUtils::writeToStream(ofs, payload);
	}

	bool fileExists(std::wstring fileName) {
		std::ifstream infile(workingDirectory + std::string("/") + StringHelp::toUtf8(fileName));
		return infile.good();
	}

	void removeFile(std::wstring fileName) {
		FileUtils::removeFile(getPathToFile(fileName));
	}

	void randomizeBuffer(Buffer &buffer) {
		buffer.setSize(buffer.getMaxSize());
		for (auto i = 0u; i < buffer.getMaxSize(); i++) {
			*(buffer.getData() + i) = static_cast<uint8_t >(rand());
		}
	}

	std::string getPathToFile(std::wstring fileName) {
		return workingDirectory + std::string("/") + StringHelp::toUtf8(fileName);
	}

	void setBufferContentToIndex(Buffer &buffer) {
		buffer.setSize(buffer.getMaxSize());
		for (auto i = 0u; i < buffer.getSize(); i++) {
			*(buffer.getData() + i) = (uint8_t) i;
		}
	}

	void setBufferToConstant(Buffer &buffer, uint8_t value) {
		buffer.setSize(buffer.getMaxSize());
		for (auto i = 0u; i < buffer.getSize(); i++) {
			*(buffer.getData() + i) = value;
		}
	}
};

#endif //TINTORRENT_FILETESTINGBASE_H
