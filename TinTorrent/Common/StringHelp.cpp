//
// Created by defacto on 12.12.16.
//

#include "StringHelp.h"

std::string StringHelp::toUtf8(const std::wstring utf16) {
	std::string outString;
	utf8::utf16to8(utf16.begin(), utf16.end(), std::back_inserter(outString));
	return outString;
}

std::wstring StringHelp::toUtf16(std::string utf8) {
	std::wstring outString;
	utf8::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(outString));
	return outString;
}

bool StringHelp::EndsWith(std::string value, std::string ending) {
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::string StringHelp::RemoveSuffix(std::string value, std::string suffix) {
	auto afterRemoval = value.substr(0, value.rfind(suffix));
	Assertions::check(value != afterRemoval, Help::Str("After removal of suffix ", suffix, " from string ",value," there was no change"));
	return afterRemoval;
}

bool StringHelp::Matches(std::string value, std::string regex) {
	std::regex reg(regex);
	return std::regex_match(value, reg);
}

std::string StringHelp::MatchesAndGetLastMatchGroup(std::string value, std::string regex) {
	std::smatch match;
	std::regex reg(regex);
	if (std::regex_search(value, match, reg) && match.size() > 1) {
		return match.str( match.size() );
	} else {
		throw std::logic_error(Help::Str("No matches found of value ",value," and regex ", regex));
	}
}

std::string StringHelp::removeSuffixAfter(std::string value, std::string suffixStart) {
	size_t lastindex = value.find_last_of(suffixStart);
	return value.substr(0, lastindex);
}
