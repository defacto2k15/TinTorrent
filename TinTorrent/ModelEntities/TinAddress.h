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
	TinAddress(std::string ipAsString, uint16_t portNumber) {
		// store this IP address in sa:
		int conversionResult = inet_pton(AF_INET, ipAsString.c_str(), &(sockaddr.sin_addr));
		Assertions::check([conversionResult](){return conversionResult==1;}, "Konwersja ip string -> ip num nie udala sie");
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port = htons(portNumber);
	}

	TinAddress( sockaddr_in sockaddr) : sockaddr(sockaddr){

	}

	sockaddr_in getSockaddr(){
		return sockaddr;
	}

	friend std::ostream &operator<<(std::ostream &os, const TinAddress &address) {
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(sockaddr.sin_addr), str, INET_ADDRSTRLEN);
		os << "sockaddr: " << str;
	}
};


#endif //TINTORRENT_TINADDRESS_H
