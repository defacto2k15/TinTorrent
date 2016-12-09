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

	uint8_t *getData() const;

	size_t getMaxSize() const;

	size_t getSize();

	void setSize( size_t newSize );

	virtual ~Buffer();
};


#endif //TINTORRENT_BUFFER_H
