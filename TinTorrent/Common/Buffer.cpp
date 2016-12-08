//
// Created by defacto on 08.12.16.
//

#include "Buffer.h"

Buffer::Buffer(char *data, size_t size) : data(data), size(size) {}

char *Buffer::getData() const {
	return data;
}

size_t Buffer::getMaxSize() const {
	return maxSize;
}

Buffer::~Buffer() {

}

void Buffer::setSize(size_t newSize) {
	Assertions::check([newSize, maxSize](){return newSize >= 0 && newSize < maxSize;},
	                  "Blad. Zly rozmiar");
	size = newSize;
}

size_t Buffer::getSize() {
	return  size;
}
