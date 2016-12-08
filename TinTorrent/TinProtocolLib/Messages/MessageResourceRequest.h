//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_MESSAGERESOURCEREQUEST_H
#define TINTORRENT_MESSAGERESOURCEREQUEST_H


#include "../../Common/InMemoryBuffer.h"
#include "Message.h"

class MessageResourceRequest : public Message{
public:
	MessageResourceRequest(InMemoryBuffer buffer);
};


#endif //TINTORRENT_MESSAGERESOURCEREQUEST_H
