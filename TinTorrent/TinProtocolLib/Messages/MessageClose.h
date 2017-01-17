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
        static CloseReason JSON_DESERIALIZATION;
        static CloseReason WRONG_SEGMENT;
        static CloseReason RESOURCE_REVERTED;
        static CloseReason RESOURCE_MISSING;

        bool operator==( const CloseReason& other)const;

        void parseJson(std::string value);
    };
private:
    CloseReason reason;
public:

    MessageClose(CloseReason reason);

    MessageClose(json &j);

    json toJson() override;

    bool operator==(const MessageClose &rhs) const;

    bool operator!=(const MessageClose &rhs) const;

    static MessageType getMessageType();

    CloseReason getReason();
};


#endif //TINTORRENT_MESSAGECLOSE_H
