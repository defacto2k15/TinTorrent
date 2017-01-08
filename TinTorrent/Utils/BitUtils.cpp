//
// Created by defacto on 22.12.16.
//

#include "BitUtils.h"

bool BitUtils::getNthBit(uint8_t value, int n) {
	return ((1 << n) & value) != 0;
}
