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
	Buffer(uint8_t *data, size_t maxSize, size_t size) : data(data),   size(size), maxSize(maxSize){};

	uint8_t *getData() const;

	size_t getMaxSize() const;

	size_t getSize();

	void setSize( size_t newSize );

	void setData( const uint8_t *inData, size_t size){
		setSize(size);
		memcpy(data, inData, size );
	}

	void setData( size_t  offset, const uint8_t *inData, size_t size){
		setSize(size+offset);
		memcpy(data+offset, inData, size );
	}

	void setData( Buffer &other, size_t offset){
		setSize( std::max( size, other.getSize()+offset));
		memcpy(data+offset, other.getData(), other.getSize() );
	}

	Buffer getBufferWithOffset(size_t offset){
		return Buffer(data+offset, maxSize-offset, size-offset);
	}

	Buffer getSubset( size_t start, size_t end){
		auto length = end - start;
		Assertions::check(end <=size, "Error: end is bigger than size of buffer");
		return Buffer( getData() + start, length, length);
	}

	bool operator==(const Buffer &rhs) const;

	bool operator!=(const Buffer &rhs) const;

	virtual ~Buffer();
};


#endif //TINTORRENT_BUFFER_H
