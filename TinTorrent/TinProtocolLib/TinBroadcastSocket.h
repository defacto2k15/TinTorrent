//
// Created by defacto on 31.12.16.
//

#ifndef TINTORRENT_TINBROADCASTSOCKET_H
#define TINTORRENT_TINBROADCASTSOCKET_H

#include <netinet/in.h>
#include <ModelEntities/Resource.h>
#include <TinProtocolLib/Messages/BroadcastMessage.h>
#include <Common/InMemoryBuffer.h>
#include <ModelEntities/TinAddress.h>
#include "SocketWrapper.h"
#include "SocketCommunicationException.h"

class TinBroadcastSocket : public SocketWrapper {
	TinAddress destinationAddress;
	InMemoryBuffer messagesBuffer;
public:
	TinBroadcastSocket( TinAddress destinationAddress ) : destinationAddress(destinationAddress), messagesBuffer(Constants::maxBroadcastMessageSize+1){
	}

	void initSocket(){
		socketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		Assertions::check<SocketCommunicationException>( [this](){ return socketDescriptor != -1;}, "Opening client broadcast socket failed");
		int broadcast = 1;
		int res = setsockopt(socketDescriptor, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast);
		Assertions::check<SocketCommunicationException>( [this, res](){ return res != -1;}, "Seting client socket to broadcasting failed");
	}

	void sendAnnounceMessage( std::vector<Resource> &resources ){
		sendMessage(resources, BroadcastMessage::BroadcastType::ANNOUNCE);
	}

	void sendRevertMessage( std::vector<Resource> &resources  ){
		sendMessage(resources, BroadcastMessage::BroadcastType::REVERT);
	}

private:
	void sendMessage(std::vector<Resource> &resources, BroadcastMessage::BroadcastType type){
		auto broadcastMessages = BroadcastMessage::generateMessages(resources, type);
		for( BroadcastMessage &message : broadcastMessages){
			writeMessageToBuffer(message);
			broadcastBuffer();
		}
	}

	void writeMessageToBuffer( BroadcastMessage &message){
		auto jsonDump = message.toJson().dump();
		messagesBuffer.setData((uint8_t *)jsonDump.c_str(), jsonDump.length()+1);
	}

	void broadcastBuffer(){
		auto mySockaddr = destinationAddress.getSockaddr();
		ssize_t sendBytes = sendto(socketDescriptor, messagesBuffer.getData(),
		                           (int)messagesBuffer.getSize(), 0,
		                           (sockaddr*)&mySockaddr, sizeof(sockaddr));
		auto &varBuffer = messagesBuffer;
		Assertions::check<SocketCommunicationException>([sendBytes, &varBuffer]{return sendBytes == (ssize_t)varBuffer.getSize();}, "Sending broadcast message failed");
		Assertions::check<SocketCommunicationException>(sendBytes != 0 , "Broadcasting: Has sent 0 bytes, as were in buffer. Propably a mistake as there was 0 in buffer");
	}

};


#endif //TINTORRENT_TINBROADCASTSOCKET_H
