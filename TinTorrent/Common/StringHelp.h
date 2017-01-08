//
// Created by defacto on 12.12.16.
//

#ifndef TINTORRENT_STRINGHELP_H
#define TINTORRENT_STRINGHELP_H

#include <string.h>
#include "utf8.h"
#include <regex>
#include <Assertions/Assertions.h>
#include <Utils/MyString.h>

//todo check if it works
class StringHelp {
public:
	static std::string toUtf8( const std::wstring utf16 );

	static std::wstring toUtf16( std::string utf8);

	static bool EndsWith( std::string value, std::string ending);

	static std::string RemoveSuffix( std::string value, std::string suffix );

	static bool Matches( std::string value, std::string regex);

	static std::string MatchesAndGetLastMatchGroup( std::string value, std::string regex);

	static std::string removeSuffixAfter( std::string value, std::string suffixStart );

};


#endif //TINTORRENT_STRINGHELP_H
