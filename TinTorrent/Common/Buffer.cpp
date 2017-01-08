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

bool Buffer::operator==(const Buffer &rhs) const {
	return (size == rhs.size) &&
	       maxSize == rhs.maxSize,
			memcmp(data, rhs.data,size) == 0;
}

bool Buffer::operator!=(const Buffer &rhs) const {
	return !(rhs == *this);
}

void Buffer::setData(const uint8_t *inData, size_t size) {
	setSize(size);
	memcpy(data, inData, size );
}

void Buffer::setData(size_t offset, const uint8_t *inData, size_t size) {
	setSize(size+offset);
	memcpy(data+offset, inData, size );
}

void Buffer::setData(Buffer &other, size_t offset) {
	setSize( std::max( size, other.getSize()+offset));
	memcpy(data+offset, other.getData(), other.getSize() );
}

Buffer Buffer::getBufferWithOffset(size_t offset) {
	return Buffer(data+offset, maxSize-offset, size-offset);
}

Buffer Buffer::getSubset(size_t start, size_t end) {
	auto length = end - start;
	Assertions::check(end <=size, "Error: end is bigger than size of buffer");
	return Buffer( getData() + start, length, length);
}
