//
// Created by defacto on 15.01.17.
//

#include <ifaddrs.h>
#include <netdb.h>
#include "OwnIpGetter.h"

TinAddress OwnIpGetter::getIp(std::string getNetworkInterfaceName) {
	struct ifaddrs *ifaddr, *ifa;
	int s;
	char host[NI_MAXHOST];

	if (getifaddrs(&ifaddr) == -1)
	{
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}


	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr == NULL)
			continue;

		s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

		if((strcmp(ifa->ifa_name,getNetworkInterfaceName.c_str())==0)&&(ifa->ifa_addr->sa_family==AF_INET))
		{
			Assertions::check(s == 0,"getnameinfo() failed" );
			freeifaddrs(ifaddr);
			return TinAddress(std::string(host), 0);
		}
	}

	Assertions::fail(Help::Str("No interface of name ",getNetworkInterfaceName," found"));
}


