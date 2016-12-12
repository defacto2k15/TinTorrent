//
// Created by defacto on 08.12.16.
//

#include "Buffer.h"
Buffer::Buffer(uint8_t *data, size_t maxSize) : data(data), size(0), maxSize(maxSize) {}

uint8_t *Buffer::getData() const {
	return data;
}

size_t Buffer::getMaxSize() const {
	return maxSize;
}

Buffer::~Buffer() {

}

void Buffer::setSize(size_t newSize) {
	auto varMaxSize = maxSize;
	Assertions::check([newSize, varMaxSize](){return newSize >= 0 && newSize <= varMaxSize;},
	                  "Blad. Zly rozmiar");
	size = newSize;
}

size_t Buffer::getSize() {
	return  size;
}
