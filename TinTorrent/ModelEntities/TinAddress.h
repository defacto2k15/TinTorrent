//
// Created by defacto on 09.12.16.
//

#ifndef TINTORRENT_TINADDRESS_H
#define TINTORRENT_TINADDRESS_H
#include <arpa/inet.h>
#include <string>
#include <netinet/in.h>
#include <ostream>
#include "../Assertions/Assertions.h"

class TinAddress {
	sockaddr_in sockaddr;
public:
	TinAddress(std::string ipAsString, uint16_t portNumber);

	TinAddress( sockaddr_in sockaddr);

	sockaddr_in getSockaddr() const;

	friend std::ostream &operator<<(std::ostream &os, const TinAddress &address);

	std::string toString();

	bool operator==(const TinAddress &rhs) const;

	bool operator!=(const TinAddress &rhs) const;
};

struct TinAddressCompare
{
	bool operator() (const TinAddress& lhs, const TinAddress& rhs) const;
};

#endif //TINTORRENT_TINADDRESS_H
