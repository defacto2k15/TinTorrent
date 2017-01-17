//
// Created by defacto on 09.12.16.
//

#include "TinConnectedClientSocket.h"

TinConnectedClientSocket::TinConnectedClientSocket(socket_descriptor_t socket) : TinConnectedSocket(socket) {
}

void TinConnectedClientSocket::sendResourceRequest(const Resource &resource) {
	MessageResourceRequest request(resource);
	serializeToBuffer(request);
	sendBuffer();
}

MessageResourceResponse::ResourceResponseValue TinConnectedClientSocket::listenForResourceResponse() {
	readToBuffer();
	auto response = deserializeFromBuffer<MessageResourceResponse >();
	return response.getValue();
}

void TinConnectedClientSocket::sendStartSendingRequest(SegmentInfo segmentInfo,
                                                       MessageStartSendingRequest::PreviousStatus prevStatus) {
	MessageStartSendingRequest request(segmentInfo, prevStatus);
	serializeToBuffer(request);
	sendBuffer();
}

SegmentResponse *TinConnectedClientSocket::listenForSegmentResponse() {
	readToBuffer();
	SegmentResponse *response = (SegmentResponse*)(buffer.getData());
	response->validateFields();
	return response;
}
