//
// Created by defacto on 08.12.16.
//

#ifndef TINTORRENT_TINPROTOCOLCOMMUNICATIONSOCKET_H
#define TINTORRENT_TINPROTOCOLCOMMUNICATIONSOCKET_H


#include "Messages/MessageResourceRequest.h"
#include "Messages/MessageEnum.h"
#include "Messages/MessageStartSendingRequest.h"
#include "SegmentInfo.h"
#include "Messages/MessageClose.h"
#include "Messages/MessageResourceResponse.h"
#include "../Assertions/Assertions.h"
#include "../Common/Typedefs.h"
#include "TinConnectedServerSocket.h"
#include <string>

class TinServerSocket : public SocketWrapper{
private:
    uint16_t portNumber;
public:
    TinServerSocket(uint16_t portNumber);

	void initSocket();

	std::shared_ptr<TinConnectedServerSocket> listenForConnections();
};



#endif //TINTORRENT_TINPROTOCOLCOMMUNICATIONSOCKET_H
