//
// Created by defacto on 08.12.16.
//

#include "TinConnectedServerSocket.h"

TinConnectedServerSocket::TinConnectedServerSocket(socket_descriptor_t socket, sockaddr_in clientAddress)
		: TinConnectedSocket(socket), clientAddress(clientAddress){
}

Resource TinConnectedServerSocket::listenForResourceRequest() {
	readToBuffer();
	auto request = deserializeFromBuffer<MessageResourceRequest>();
	return request.getResource();
}

void TinConnectedServerSocket::sendResourceResponse(MessageResourceResponse::ResourceResponseValue responseValue) {
	MessageResourceResponse response(responseValue);
	serializeToBuffer(response);
	sendBuffer();
}

MessageStartSendingRequest TinConnectedServerSocket::listenForStartSendingRequest() {
	readToBuffer();
	return deserializeFromBuffer<MessageStartSendingRequest >();
}

void TinConnectedServerSocket::sendSegmentResponse(SegmentInfo segmentInfo, uint8_t *payload) {
	SegmentResponse::writeSegmentResponseToBuffer( segmentInfo, payload, buffer);
	sendBuffer();
}

TinAddress TinConnectedServerSocket::getClientAddress() {
	return TinAddress(clientAddress);
}
