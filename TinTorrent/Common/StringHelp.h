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
	static std::string toUtf8( const std::wstring utf16 ){
		std::string outString;
		utf8::utf16to8(utf16.begin(), utf16.end(), std::back_inserter(outString));
		return outString;
	}

	static std::wstring toUtf16( std::string utf8){
		std::wstring outString;
		utf8::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(outString));
		return outString;
	}

	static bool EndsWith( std::string value, std::string ending){
		if (ending.size() > value.size()) return false;
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}

	static std::string RemoveSuffix( std::string value, std::string suffix ){
		auto afterRemoval = value.substr(0, value.rfind(suffix));
		Assertions::check(value != afterRemoval, Help::Str("After removal of suffix ", suffix, " from string ",value," there was no change"));
		return afterRemoval;
	}

	static bool Matches( std::string value, std::string regex){
		std::regex reg(regex);
		return std::regex_match(value, reg);
	}

	static std::string MatchesAndGetLastMatchGroup( std::string value, std::string regex){
		std::smatch match;
		std::regex reg(regex);
		if (std::regex_search(value, match, reg) && match.size() > 1) {
			return match.str( match.size() );
		} else {
			throw std::logic_error(Help::Str("No matches found of value ",value," and regex ", regex));
		}
	}

};


#endif //TINTORRENT_STRINGHELP_H
