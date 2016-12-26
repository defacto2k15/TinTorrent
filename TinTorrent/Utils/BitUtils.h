//
// Created by defacto on 22.12.16.
//

#ifndef TINTORRENT_BITUTILS_H
#define TINTORRENT_BITUTILS_H


#include <cstdint>

class BitUtils {
public:
	static bool getNthBit( uint8_t value, int n){
		return ((1 << n) & value) != 0;
	}
};


#endif //TINTORRENT_BITUTILS_H
