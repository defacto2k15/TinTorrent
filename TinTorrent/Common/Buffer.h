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

	void setData( const uint8_t *inData, size_t size);

	void setData( size_t  offset, const uint8_t *inData, size_t size);

	void setData( Buffer &other, size_t offset);

	Buffer getBufferWithOffset(size_t offset);

	Buffer getSubset( size_t start, size_t end);

	bool operator==(const Buffer &rhs) const;

	bool operator!=(const Buffer &rhs) const;

	virtual ~Buffer();
};


#endif //TINTORRENT_BUFFER_H
