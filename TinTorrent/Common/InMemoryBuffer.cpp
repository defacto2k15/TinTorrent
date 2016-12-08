//
// Created by defacto on 08.12.16.
//

#include "InMemoryBuffer.h"

InMemoryBuffer::InMemoryBuffer(char *data, int size) : Buffer(data, size) {}

InMemoryBuffer::~InMemoryBuffer() {
	delete data;
}
