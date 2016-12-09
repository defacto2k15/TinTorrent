//
// Created by defacto on 08.12.16.
//

#include "Buffer.h"

Buffer::Buffer(uint8_t *data, size_t size) : data(data), size(size) {}

uint8_t *Buffer::getData() const {
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
