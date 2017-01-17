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
#include <ostream>

class MessageEnum {
private:
    std::string value;
protected:
    MessageEnum ( const std::string &value );
    MessageEnum ();
public:
    std::string getValue() const;

    bool operator==( const MessageEnum &other)const;

    void parseJson(std::string jsonValue, std::vector<MessageEnum> possibleValues );

    ~MessageEnum();

    friend std::ostream &operator<<(std::ostream &os, const MessageEnum &anEnum);
};


#endif //TINTORRENT_MESSAGEENUM_H
