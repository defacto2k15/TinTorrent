//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_ASSERTIONS_H
#define TINTORRENT_ASSERTIONS_H


#include <functional>
#include <cstring>

class Assertions {
public:
    static void check( std::function<bool()> funcToCheck, std::string errMessage ){
	    check(funcToCheck(), errMessage);
    }
	static void check( bool value, std::string errMessage ){
		if(!value){
			errMessage.append( std::string(" Errno is ")+std::string(strerror(errno)));
			throw AssertionException(errMessage);
		}
	}

    class AssertionException : public std::runtime_error{
    public:
        AssertionException(std::string message ) : std::runtime_error(message){
        }
    };
};


#endif //TINTORRENT_ASSERTIONS_H
