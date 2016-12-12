//
// Created by defacto on 12.12.16.
//

#ifndef TINTORRENT_STRINGHELP_H
#define TINTORRENT_STRINGHELP_H

#include <string.h>
#include "utf8.h"

//todo check if it works
class StringHelp {
public:
	static std::string toUtf8(std::wstring utf16 ){
		std::string outString;
		utf8::utf16to8(utf16.begin(), utf16.end(), std::back_inserter(outString));
		return outString;
	}

	static std::wstring toUtf16( std::string utf8){
		std::wstring outString;
		utf8::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(outString));
		return outString;
	}
};


#endif //TINTORRENT_STRINGHELP_H
