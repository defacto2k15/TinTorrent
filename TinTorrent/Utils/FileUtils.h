//
// Created by defacto on 26.12.16.
//

#ifndef TINTORRENT_FILEUTILS_H
#define TINTORRENT_FILEUTILS_H

#include <string>
#include <algorithm>
#include <Assertions/Assertions.h>
#include <Common/Buffer.h>
#include <ios>
#include "MyString.h"
#include <fstream>
#include <Common/Constants.h>

class FileUtils {
public:
	static std::string getFilenameFromPath(std::string pathname){
		return std::string(
				std::find_if( pathname.rbegin(), pathname.rend(),
				              MatchPathSeparator() ).base(),
				pathname.end() );
	}

	struct MatchPathSeparator
	{
		bool operator()( char ch ) const
		{
			return ch == '\\' || ch == '/';
		}
	};

	static void renameFile(std::string oldName, std::string newName ){
		int res = std::rename( oldName.c_str(), newName.c_str());
		Assertions::check(res == 0, Help::Str("Renaming file ",oldName," to ",newName," failed"));
	}

	static void removeFile(std::string filePath ){
		auto res = std::remove(filePath.c_str());
		Assertions::check(res == 0, Help::Str("Removing file ",filePath," failed"));
	}

	static size_t readContent( std::string path, Buffer &buffer, size_t offset, size_t maxPayloadToRead){
		std::ifstream ifs (path.c_str(), std::fstream::binary);
		Assertions::check( !(!ifs), Help::Str("Opening of file with path ",path," failed."));

		ifs.seekg(offset);
		ifs.read((char*)buffer.getData(), maxPayloadToRead);
		auto readCount = ifs.gcount();
		Assertions::check( readCount != 0, Help::Str("Problem: Read 0 bytes from file of path ", path));
		buffer.setSize((size_t)readCount);
		return (size_t)readCount;
	}

	static void writeContent( std::string path, Buffer buffer, size_t offset ){
		std::ofstream ofs(path.c_str(), std::ios::in | std::ios::out | std::ios::binary);
		Assertions::check( !(!ofs), Help::Str("Opening of file with path ",path," failed."));

		ofs.seekp(offset);
		ofs.write((char*)buffer.getData(), buffer.getSize());
	}

	static void createFile( std::string path, size_t size){
		std::ofstream ofs(path.c_str(), std::ios::out | std::ios::binary);
		Assertions::check( !(!ofs), Help::Str("Opening of file with path ",path," failed."));

		ofs.seekp( size-1);
		ofs.write("", 1);
	}

	static void createZeroedFile( std::string path, size_t size){
		std::ofstream ofs(path.c_str(),  std::ios::out | std::ios::binary);
		Assertions::check( !(!ofs), Help::Str("Opening of file with path ",path," failed."));

		for (int i = 0; i < size; ++i)
		{
			char zero = 0;
			ofs.write(&zero, sizeof(char));
		}
	}

};


#endif //TINTORRENT_FILEUTILS_H
