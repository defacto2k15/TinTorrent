//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_MESSAGESTARTSENDINGREQUEST_H
#define TINTORRENT_MESSAGESTARTSENDINGREQUEST_H


#include "MessageEnum.h"
#include "../../Common/InMemoryBuffer.h"
#include "Message.h"
#include "../SegmentInfo.h"
#include "MessageType.h"

using json = nlohmann::json;

class MessageStartSendingRequest : public Message{
public:
    class PreviousStatus : public MessageEnum {
    public:
        PreviousStatus(const std::string &value);;
        explicit PreviousStatus(json jsonValue);
        static PreviousStatus OK;
        static PreviousStatus FIRST;
        static PreviousStatus WRONG_SEGMENT;
        static PreviousStatus WRONG_SIZE;

        void parseJson(std::string value);
    };

private:
    PreviousStatus previous;
    SegmentInfo segmentInfo;
public:

    explicit  MessageStartSendingRequest( json &j);

    MessageStartSendingRequest( SegmentInfo segment, PreviousStatus status );

    json toJson() override;

    PreviousStatus getPreviousStatus();

    SegmentInfo getSegmentInfo();

    bool operator==(const MessageStartSendingRequest &other) const;

    static MessageType getMessageType();

};


#endif //TINTORRENT_MESSAGESTARTSENDINGREQUEST_H
