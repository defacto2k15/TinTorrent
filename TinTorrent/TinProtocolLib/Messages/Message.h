//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_MESSAGE_H
#define TINTORRENT_MESSAGE_H


#include <json.hpp>
#include "../../Common/Buffer.h"
using json = nlohmann::json;

class Message {

public:
	virtual json toJson()=0;
	Message(){
	}
};


#endif //TINTORRENT_MESSAGE_H
