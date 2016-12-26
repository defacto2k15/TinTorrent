//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_MESSAGECLOSE_H
#define TINTORRENT_MESSAGECLOSE_H


#include "MessageEnum.h"
#include "Message.h"
#include "MessageType.h"

class MessageClose  : public Message{
public:
    class CloseReason : public  MessageEnum{
        CloseReason(const std::string &value);
    public:
        CloseReason();;
        static CloseReason OK;

        bool operator==( const CloseReason& other)const{
            return MessageEnum::operator==(other);
        }

        void parseJson(std::string value){
            MessageEnum::parseJson(value, {OK});
        }
    };
private:
    CloseReason reason;
public:

    MessageClose(CloseReason reason);

    MessageClose(json &j);

    json toJson() override;

    bool operator==(const MessageClose &rhs) const;

    bool operator!=(const MessageClose &rhs) const;

    static MessageType getMessageType(){
        return MessageType::CLOSE;
    }
};


#endif //TINTORRENT_MESSAGECLOSE_H