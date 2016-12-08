//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_MESSAGECLOSE_H
#define TINTORRENT_MESSAGECLOSE_H


#include "MessageEnum.h"
#include "Message.h"

class MessageClose  : public Message{

public:
    MessageClose(CloseReason reason);

    class CloseReason : MessageEnum{
    public:
        static CloseReason OK;
        CloseReason(const std::string &value);
    };
};


#endif //TINTORRENT_MESSAGECLOSE_H
