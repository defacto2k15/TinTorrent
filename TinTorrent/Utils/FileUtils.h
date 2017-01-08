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
	static std::string getFilenameFromPath(std::string pathname);

	struct MatchPathSeparator
	{
		bool operator()( char ch ) const
		{
			return ch == '\\' || ch == '/';
		}
	};

	static void renameFile(std::string oldName, std::string newName );

	static void removeFile(std::string filePath );

	static size_t readContent( std::string path, Buffer &buffer, size_t offset, size_t maxPayloadToRead);

	static void writeContent( std::string path, Buffer buffer, size_t offset );

	static void createFile( std::string path, size_t size);

	static void createZeroedFile( std::string path, size_t size);

};


#endif //TINTORRENT_FILEUTILS_H
