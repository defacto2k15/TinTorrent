//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_INMEMORYBUFFER_H
#define TINTORRENT_INMEMORYBUFFER_H


#include "Buffer.h"

class InMemoryBuffer : public Buffer {
public:
	InMemoryBuffer(size_t  size);
	InMemoryBuffer( InMemoryBuffer &&other) : Buffer(std::move(other.data), std::move(other.size)){
		other.data = nullptr;
		other.size = 0;
		other.maxSize = 0;
	}

	InMemoryBuffer(InMemoryBuffer const&) = delete;
	InMemoryBuffer& operator=(InMemoryBuffer const&) = delete;

	virtual ~InMemoryBuffer();
};


#endif //TINTORRENT_INMEMORYBUFFER_H
