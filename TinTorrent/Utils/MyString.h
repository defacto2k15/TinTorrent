//
// Created by defacto on 22.12.16.
//

#ifndef TINTORRENT_MYSTRING_H
#define TINTORRENT_MYSTRING_H

#include <string>
#include <sstream>

class Help {
	template<typename T>
	struct identity { typedef T type; };
public:
	template<typename T, class ... Rest>
	static std::string 	Str( T firstElem, Rest ... args ){
		std::stringstream ss;
		Str(ss, firstElem, args...);
		return ss.str();
	}

private:
	template<typename T>
	static void Str( std::stringstream &ss, T onlyElem ){
		addToStream(ss, onlyElem, identity<T>());
	}

	template<typename T, class ... Rest>
	static void Str( std::stringstream &ss,  T firstElem, Rest ... args){
		addToStream(ss, firstElem, identity<T>());
		Str( ss, args ... );
	}


#pragma GCC diagnostic ignored "-Wunused-parameter"
	template<typename  T>
	static void addToStream( std::stringstream &ss, T onlyElem, identity<const char*> id ){
		ss << std::string(onlyElem);
	}
	template<typename T, typename  ID>
	static void addToStream( std::stringstream &ss, T &onlyElem, identity<ID>){
		ss << onlyElem;
	}


};


#endif //TINTORRENT_MYSTRING_H
