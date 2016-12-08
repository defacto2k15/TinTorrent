//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_ASSERTIONS_H
#define TINTORRENT_ASSERTIONS_H


#include <functional>

class Assertions {
public:
    static void check( std::function<bool()> funcToCheck, std::string errMessage ){
       if( !funcToCheck()){
           throw new AssertionException(errMessage);
       }
    }

    class AssertionException : public std::runtime_error{
    public:
        AssertionException(std::string message ) : std::runtime_error(message){
        }
    };
};


#endif //TINTORRENT_ASSERTIONS_H
