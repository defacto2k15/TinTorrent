//
// Created by defacto on 09.12.16.
//

#include "TinConnectedSocket.h"

void TinConnectedSocket::readToBuffer() {
	ssize_t allReadBytes = 0;
	uint16_t declaredSize = 0;

	while( true ) {
		auto readSize = recv( socket, buffer.getData()+allReadBytes, buffer.getMaxSize()-allReadBytes, 0);
		Assertions::check<SocketCommunicationException>([readSize](){ return readSize > 0;},
		                                                Help::Str("Recieving message failed. Read ",readSize," bytes"));

		if (allReadBytes == 0) { // first reading'
			assertBufferHasGoodConstantPrefix();
			if( constantPrefixIsFromSegmentResponse()){
				declaredSize = getSizeFromBuffer() + SegmentResponse::prefixSize;
			} else {
				declaredSize = getSizeFromBuffer();
			}
			if (declaredSize == readSize) { //ok, readAllBytes
				break;
			}
			allReadBytes += readSize;
		} else {
			allReadBytes += readSize;
			Assertions::check(allReadBytes <= declaredSize,
			                  Help::Str("Have read more bytes than declared. Declared ",declaredSize," got ",allReadBytes) );
			if( allReadBytes == declaredSize){
				break;
			}
		}
	}
	buffer.setSize((size_t )declaredSize);
	checkIfRecievedMessageClose();
}

void TinConnectedSocket::checkIfRecievedMessageClose() {
	uint8_t  *bufferPtr = buffer.getData();
	if( bufferPtr[0] == 0xff && bufferPtr[1] == 0xfd ) //todo from configuration
	{
		try {
			json j = getJsonFromBuffer();
			MessageType type;
			type.parseJson(j["Type"]);
			if (type == MessageType::CLOSE) {
				MessageClose close = deserializeFromBuffer<MessageClose>();
				throw MessageCloseException(close);
			}
		} catch( MessageCloseException &e) {
			throw e;
		}catch( std::exception &e ){
			//ignore
		}
	}
}

void TinConnectedSocket::sendBuffer() {
	ssize_t bytesToSend = buffer.getSize();
	while (bytesToSend > 0){
		ssize_t sendBytes = send(socket, buffer.getData()+buffer.getSize()-bytesToSend, bytesToSend , 0);
		Assertions::check<SocketCommunicationException>(sendBytes > 0, Help::Str( "Error sending. Sent ",sendBytes," bytes") );
		Assertions::check<SocketCommunicationException>(
				sendBytes <= bytesToSend, Help::Str( "Error sending. Sent ",sendBytes," bytes when wonted to send only",bytesToSend) );
		bytesToSend -= sendBytes;
	}
}

void TinConnectedSocket::assertBufferHasGoodConstantPrefix() {
	Assertions::check<SocketCommunicationException>( buffer.getData()[0] == 0xff && (buffer.getData()[1] == 0xfd || buffer.getData()[1] == 0xff),
	                                                 Help::Str("Received data has bad constants prefix. First 4bytes are ", *((uint32_t*)buffer.getData())));
}

void TinConnectedSocket::assertBufferHasGoodSizePrefix() {
	uint16_t sizePrefix = *((uint16_t *)(buffer.getData()+2));
	Assertions::check<SocketCommunicationException>( sizePrefix == buffer.getSize(),
	                                                 Help::Str("SizePrefix ", sizePrefix, "is not equal to buffer content length ",buffer.getSize(),
	                                                           ". First 4 bytes are ", *((uint32_t*)buffer.getData())));
}

bool TinConnectedSocket::constantPrefixIsFromSegmentResponse() {
	return buffer.getData()[0] == 0xff && buffer.getData()[1] == 0xff;
}

TinConnectedSocket::TinConnectedSocket(socket_descriptor_t socket) : socket(socket ), buffer( Constants::segmentSize+300){}

void TinConnectedSocket::closeConnection(MessageClose::CloseReason closeReason) {
	MessageClose messageClose(closeReason);
	serializeToBuffer(messageClose);
	sendBuffer();
}

json TinConnectedSocket::getJsonFromBuffer() {
	Buffer bufferToDeserialize = buffer.getBufferWithOffset(serializedMessageSizeOffset);
	std::string jsonString((const char*) (buffer.getData()+serializedMessageSizeOffset));
	Assertions::check<SocketCommunicationException>([&jsonString, this](){ return jsonString.length() < buffer.getSize();}, //todo change to equal
	                                                Help::Str("Message deserialization. InString ",jsonString.length()," bigger than buffer ",buffer.getSize()));
	json j;
	try {
		j = json::parse(jsonString);
	} catch( std::exception &e){
		throw SocketCommunicationException(e);
	}
	return j;
}

uint16_t TinConnectedSocket::getConstantPrefixFromBuffer() {
	return *(uint16_t*)buffer.getData();
}

uint16_t TinConnectedSocket::getSizeFromBuffer() {
	return *(uint16_t*)(buffer.getData()+2);
}
