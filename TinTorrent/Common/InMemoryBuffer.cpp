//
// Created by defacto on 08.12.16.
//

#include "InMemoryBuffer.h"

InMemoryBuffer::InMemoryBuffer(size_t size) : Buffer(new uint8_t[size], size) {}

InMemoryBuffer::~InMemoryBuffer() {
	delete []data;
}
