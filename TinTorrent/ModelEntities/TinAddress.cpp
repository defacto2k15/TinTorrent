//
// Created by defacto on 09.12.16.
//

#include "TinAddress.h"

bool TinAddress::operator==(const TinAddress &rhs) const {
	return  TinAddressCompare()(*this, rhs);
}

bool TinAddress::operator!=(const TinAddress &rhs) const {
	return !(rhs == *this);
}
