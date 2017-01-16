//
// Created by defacto on 15.01.17.
//

#ifndef TINTORRENT_OWNIPGETTER_H
#define TINTORRENT_OWNIPGETTER_H

#include <ModelEntities/TinAddress.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <Utils/MyString.h>
#include <Common/Constants.h>


class OwnIpGetter {
public:
	static TinAddress getIp(std::string getNetworkInterfaceName);
};


#endif //TINTORRENT_OWNIPGETTER_H
