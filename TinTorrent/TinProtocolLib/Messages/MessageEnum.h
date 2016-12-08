//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_MESSAGEENUM_H
#define TINTORRENT_MESSAGEENUM_H
#include <string>

class MessageEnum {
private:
    std::string value;
protected:
    MessageEnum ( std::string value ) : value(value){}
public:
    std::string getValue(){
        return value;
    }
};


#endif //TINTORRENT_MESSAGEENUM_H
