//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_THREADTINUDPLISTENINGSOCKET_H
#define TINTORRENT_THREADTINUDPLISTENINGSOCKET_H


#include <TinProtocolLib/TinListeningUdpSocket.h>
class Kernel;

class ThreadTinUdpListeningSocket : public ActionQueue<ThreadTinUdpListeningSocket>{
	LogBase log;
	TinListeningUdpSocket udpSocket;
	Kernel &kernel;
public:
	ThreadTinUdpListeningSocket( Kernel &kernel);

	void init();

	void listenForBroadcasts();

	void internalKillYourself() override;
};


#endif //TINTORRENT_THREADTINUDPLISTENINGSOCKET_H
