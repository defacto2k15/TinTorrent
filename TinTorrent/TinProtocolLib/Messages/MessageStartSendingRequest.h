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
        PreviousStatus(const std::string &value) : MessageEnum(value){};
        explicit PreviousStatus(json jsonValue)  {
            parseJson(jsonValue);
        }
        static PreviousStatus OK;
        static PreviousStatus FIRST;
        static PreviousStatus WRONG_SEGMENT;
        static PreviousStatus WRONG_SIZE;

        void parseJson(std::string value){
            MessageEnum::parseJson(value, {OK, FIRST, WRONG_SEGMENT, WRONG_SIZE});
        }
    };

private:
    PreviousStatus previous;
    SegmentInfo segmentInfo;
public:

    explicit  MessageStartSendingRequest( json &j) : previous(j["Previous"]), segmentInfo(j["SegmentInfo"]){
        MessageType type;
        type.parseJson(j["Type"]);
        Assertions::check([&](){ return type==MessageType::START_SENDING_REQUEST;}, "StartSendingRequest deserialization. Type in wrong");
    }

    MessageStartSendingRequest( SegmentInfo segment, PreviousStatus status ) : segmentInfo(segment), previous(status){
    }

    json toJson() override {
        json j  = {
                {"Type", MessageType::START_SENDING_REQUEST.getValue()},
                {"Previous", previous.getValue()},
                {"SegmentInfo", segmentInfo.toJson()}
        };
        return j;
    }

    PreviousStatus getPreviousStatus() {
        return previous;
    }

    SegmentInfo getSegmentInfo() {
        return segmentInfo;
    }

    bool operator==(const MessageStartSendingRequest &other) const {
        return this->segmentInfo == other.segmentInfo &&
               this->previous == other.previous;
    }

    static MessageType getMessageType(){
        return MessageType::START_SENDING_REQUEST;
    }

};


#endif //TINTORRENT_MESSAGESTARTSENDINGREQUEST_H
