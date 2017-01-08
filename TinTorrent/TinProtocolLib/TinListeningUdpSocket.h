//
// Created by defacto on 31.12.16.
//

#ifndef TINTORRENT_TINLISTENINGUDPSOCKET_H
#define TINTORRENT_TINLISTENINGUDPSOCKET_H


#include <netinet/in.h>
#include <Common/InMemoryBuffer.h>
#include <Common/Constants.h>
#include <TinProtocolLib/Messages/BroadcastMessage.h>
#include <ModelEntities/TinAddress.h>
#include "SocketWrapper.h"
#include "SocketCommunicationException.h"

class TinListeningUdpSocket : public SocketWrapper{
	InMemoryBuffer messagesBuffer;
public:
	TinListeningUdpSocket() : messagesBuffer(Constants::maxBroadcastMessageSize){
	}

	void initSocket(){
		socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
		Assertions::check<SocketCommunicationException>( [this](){ return socketDescriptor != -1;}, "Opening client broadcast socket failed");

		struct sockaddr_in si_me;
		si_me.sin_family = AF_INET;
		si_me.sin_port = htons(Constants::broadcastPort);
		si_me.sin_addr.s_addr = htonl(INADDR_ANY);

		int bindResult = bind(socketDescriptor , (struct sockaddr*)&si_me, sizeof(si_me) );
		Assertions::check<SocketCommunicationException>(bindResult != -1, "Binding socket in broadcast listener failed");
	}

	std::pair<BroadcastMessage, TinAddress> listenForMessages(){
		auto senderAddress = readToBuffer();
		std::string jsonString((const char*) (messagesBuffer.getData()));
		Assertions::check<SocketCommunicationException>([&jsonString, this](){ return jsonString.length() < messagesBuffer.getSize();}, //todo change to equal
		                  "BroadcastMessage deserialization. InString bigger than buffer");
		json j = json::parse(jsonString);
		return std::make_pair(BroadcastMessage(j), TinAddress(senderAddress));
	}
private:
	sockaddr_in readToBuffer(){
		sockaddr_in other;
		socklen_t size;
		ssize_t readSize = recvfrom( socketDescriptor, messagesBuffer.getData(), messagesBuffer.getMaxSize(), 0, (sockaddr*)&other, &size);
		Assertions::check<SocketCommunicationException>( readSize != -1 && readSize != 0, "Recieving message failed");
		messagesBuffer.setSize((size_t )readSize);
		return other;
	}


};


#endif //TINTORRENT_TINLISTENINGUDPSOCKET_H
