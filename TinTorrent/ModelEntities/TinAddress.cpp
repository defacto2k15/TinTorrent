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

TinAddress::TinAddress(std::string ipAsString, uint16_t portNumber) {
	// store this IP address in sa:
	int conversionResult = inet_pton(AF_INET, ipAsString.c_str(), &(sockaddr.sin_addr));
	Assertions::check([conversionResult](){return conversionResult==1;}, "Konwersja ip string -> ip num nie udala sie");
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(portNumber);
}

TinAddress::TinAddress(sockaddr_in sockaddr) : sockaddr(sockaddr){

}

sockaddr_in TinAddress::getSockaddr() const {
	return sockaddr;
}

std::ostream &operator<<(std::ostream &os, const TinAddress &address) {
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(address.sockaddr.sin_addr), str, INET_ADDRSTRLEN);
	os << "sockaddr: " << str;
	return os;
}

bool TinAddressCompare::operator()(const TinAddress &lhs, const TinAddress &rhs) const {
	return lhs.getSockaddr().sin_addr.s_addr < rhs.getSockaddr().sin_addr.s_addr;
}
