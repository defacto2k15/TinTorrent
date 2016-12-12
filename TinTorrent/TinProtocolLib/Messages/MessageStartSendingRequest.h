//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_MESSAGESTARTSENDINGREQUEST_H
#define TINTORRENT_MESSAGESTARTSENDINGREQUEST_H


#include "MessageEnum.h"
#include "../../Common/InMemoryBuffer.h"
#include "Message.h"
#include "../SegmentInfo.h"

class MessageStartSendingRequest : public Message{
public:
    class Type : MessageEnum {
    public:
        Type(const std::string &value) : MessageEnum(value){};
        static Type OK;
        static Type FIRST;
        static Type WRONG_SEGMENT;
        static Type WRONG_SIZE;
    };

    MessageStartSendingRequest( Buffer &buffer){
        //todo
    }

    MessageStartSendingRequest( SegmentInfo segment ){
        //todo
    }

    std::pair<SegmentInfo, Type> getInfoPair() {
	    //todo
    }

    void serializeTo(Buffer &buffer) override {
        //todo
    }

};


#endif //TINTORRENT_MESSAGESTARTSENDINGREQUEST_H
