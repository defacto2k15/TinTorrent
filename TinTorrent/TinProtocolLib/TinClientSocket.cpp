//
// Created by defacto on 09.12.16.
//

#include "TinClientSocket.h"

TinClientSocket::TinClientSocket(socket_descriptor_t socket, const TinAddress &addressToConnect) : socket(socket),
                                                                                                   addressToConnect(
		                                                                                                   addressToConnect) {}
