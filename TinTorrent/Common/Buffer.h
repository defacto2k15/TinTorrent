//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_BUFFER_H
#define TINTORRENT_BUFFER_H


#include "../Assertions/Assertions.h"

class Buffer  {
protected:
	uint8_t * data;
	size_t size;
	size_t maxSize;
public:
	Buffer(uint8_t *data, size_t maxSize);
	Buffer(uint8_t *data, size_t maxSize, size_t size) : data(data), maxSize(maxSize), size(size){};

	uint8_t *getData() const;

	size_t getMaxSize() const;

	size_t getSize();

	void setSize( size_t newSize );

	void setData( const uint8_t *inData, size_t size){
		setSize(size);
		memcpy(data, inData, size );
	}

	Buffer getBufferWithOffset(size_t offset){
		return Buffer(data+offset, maxSize-offset, size-offset);
	}

	virtual ~Buffer();
};


#endif //TINTORRENT_BUFFER_H
