//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_MESSAGE_H
#define TINTORRENT_MESSAGE_H


#include "../../Common/Buffer.h"

class Message {
protected:
	virtual void serializeTo( Buffer &buffer)=0;

};


#endif //TINTORRENT_MESSAGE_H
