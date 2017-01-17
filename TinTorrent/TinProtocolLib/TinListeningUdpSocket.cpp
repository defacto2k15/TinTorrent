//
// Created by defacto on 31.12.16.
//

#include "TinListeningUdpSocket.h"

TinListeningUdpSocket::TinListeningUdpSocket() : messagesBuffer(Constants::maxBroadcastMessageSize){
}

void TinListeningUdpSocket::initSocket() {
	socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
	Assertions::check<SocketCommunicationException>( [this](){ return socketDescriptor != -1;}, "Opening client broadcast socket failed");

	struct sockaddr_in si_me;
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(Constants::broadcastPort);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	int bindResult = bind(socketDescriptor , (struct sockaddr*)&si_me, sizeof(si_me) );
	Assertions::check<SocketCommunicationException>(bindResult != -1, "Binding socket in broadcast listener failed");
}

std::pair<BroadcastMessage, TinAddress> TinListeningUdpSocket::listenForMessages() {
	auto senderUdpAddress = readToBuffer();
	std::string jsonString((const char*) (messagesBuffer.getData()));
	Assertions::check<SocketCommunicationException>([&jsonString, this](){ return jsonString.length() < messagesBuffer.getSize();}, //todo change to equal
	                                                "BroadcastMessage deserialization. InString bigger than buffer");
	json j = json::parse(jsonString);

	sockaddr_in senderTcpAddress;
	senderTcpAddress.sin_family = AF_INET;
	senderTcpAddress.sin_addr.s_addr = senderUdpAddress.sin_addr.s_addr;
	senderTcpAddress.sin_port = htons(Constants::communicationPort);
	return std::make_pair(BroadcastMessage(j), TinAddress(senderTcpAddress));
}

sockaddr_in TinListeningUdpSocket::readToBuffer() {
	sockaddr_in other;
	socklen_t size;
	ssize_t readSize = recvfrom( socketDescriptor, messagesBuffer.getData(), messagesBuffer.getMaxSize(), 0, (sockaddr*)&other, &size);
	Assertions::check<SocketCommunicationException>( readSize != -1 && readSize != 0, "Recieving message failed");
	messagesBuffer.setSize((size_t )readSize);
	return other;
}
