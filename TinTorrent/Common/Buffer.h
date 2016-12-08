//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_BUFFER_H
#define TINTORRENT_BUFFER_H


#include "../Assertions/Assertions.h"

class Buffer  {
protected:
	char * data;
	size_t size;
	size_t maxSize;
public:
	Buffer(char *data, size_t maxSize);

	char *getData() const;

	size_t getMaxSize() const;

	size_t getSize();

	void setSize( size_t newSize );

	virtual ~Buffer();
};


#endif //TINTORRENT_BUFFER_H
