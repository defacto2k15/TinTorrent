//
// Created by defacto on 09.12.16.
//

#ifndef TINTORRENT_TINCONNECTEDSOCKET_H
#define TINTORRENT_TINCONNECTEDSOCKET_H


#include <sys/socket.h>
#include "SocketWrapper.h"
#include "Messages/MessageResourceRequest.h"
#include "Messages/MessageClose.h"

class TinConnectedSocket : public SocketWrapper {
	socket_descriptor_t  socket;
	// todo from conf
	const size_t serializedMessageSizeOffset = 2;
protected:
	InMemoryBuffer buffer;

	void readToBuffer(){
		ssize_t readSize = recv( socket, buffer.getData(), buffer.getMaxSize(), 0);
		Assertions::check([readSize](){ return readSize != -1;}, "Recieving message failed");
		buffer.setSize((size_t )readSize);
	}

	void sendBuffer(){
		ssize_t sendBytes = send(socket, buffer.getData(), buffer.getSize(), 0);
		auto &varBuffer = buffer;
		Assertions::check([sendBytes, &varBuffer]{return sendBytes == varBuffer.getSize();}, "Sending failed");
		Assertions::check(sendBytes != 0 , "Has sent 0 bytes, as were in buffer. Propably a mistake as there was 0 in buffer");
	}

	// todo there should be constantId taken into consideration
	void serializeToBuffer( Message &message ){
		Buffer bufferToSerializeMessage = buffer.getBufferWithOffset(serializedMessageSizeOffset);
		message.serializeTo(bufferToSerializeMessage);
		size_t serializedDataSize = bufferToSerializeMessage.getSize();
		//todo 4096 from configuration
		Assertions::check(serializedDataSize <= 4096, "Serialized data is too big to fit in message");
		*((uint16_t *)buffer.getData()) = (uint16_t)(serializedDataSize+2);
		buffer.setSize( serializedDataSize+2);
	}

	template< typename T>
	T deserializeFromBuffer(){
		assertBufferHasSizePrefix();
		Buffer bufferToDeserialize = buffer.getBufferWithOffset(serializedMessageSizeOffset);
		return T(bufferToDeserialize);
	}

	void assertBufferHasSizePrefix(){
		// todo UWAGA : na razie sizePrefix nie bierze pod uwage wielkosci samego siebie, czyli tych 2b
		uint16_t sizePrefix = *((uint16_t *)buffer.getData());
		//todo throw other, business logic exception
		Assertions::check( sizePrefix == buffer.getSize(), "SizePrefix is not equal to buffer content length");
	}

public:
	// TODO buffer size from configuration
	TinConnectedSocket( socket_descriptor_t socket ) : socket(socket ), buffer( 5000){};

	void closeConnection( MessageClose::CloseReason closeReason ){
		MessageClose messageClose(closeReason);
		serializeToBuffer(messageClose);
		sendBuffer();
	}
};


#endif //TINTORRENT_TINCONNECTEDSOCKET_H
