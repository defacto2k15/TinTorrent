//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_MESSAGEENUM_H
#define TINTORRENT_MESSAGEENUM_H
#include <string>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <memory>

class MessageEnum {
private:
    std::string value;
protected:
    MessageEnum ( const std::string &value ) :value(value){
    }
    MessageEnum (){
    }
public:
    std::string getValue() const{
        if( value.length()==0){
            throw std::logic_error("Message enum zainicjowany domyslnym konstruktorem, uzytecznym tylko do parsowania");
        }
        return value;
    }

    bool operator==( const MessageEnum &other)const{
	    return value == other.value;
    }

    void parseJson(std::string jsonValue, std::vector<MessageEnum> possibleValues ){
        auto x = std::find_if(begin(possibleValues), end(possibleValues),
                     [jsonValue](MessageEnum & messageEnum){ return messageEnum.getValue()==jsonValue;});
        if( x == possibleValues.end()){
            throw std::logic_error(std::string("Blad przy parsowaniu MessageEnum. Nie ma takiego o wartosci: ")+jsonValue);
        }
        value = jsonValue;
    }

    ~MessageEnum(){
        int z = 33;//todo
    }
};


#endif //TINTORRENT_MESSAGEENUM_H
