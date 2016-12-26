//
// Created by defacto on 26.12.16.
//

#ifndef TINTORRENT_FILEUTILS_H
#define TINTORRENT_FILEUTILS_H

#include <string>
#include <algorithm>
#include <Assertions/Assertions.h>
#include "MyString.h"

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
};


#endif //TINTORRENT_FILEUTILS_H
