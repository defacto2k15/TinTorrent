//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_INMEMORYBUFFER_H
#define TINTORRENT_INMEMORYBUFFER_H


#include "Buffer.h"

class InMemoryBuffer : public Buffer {
public:
	InMemoryBuffer(char *data, int size);

	virtual ~InMemoryBuffer();
};


#endif //TINTORRENT_INMEMORYBUFFER_H
