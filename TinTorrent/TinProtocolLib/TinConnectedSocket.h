//
// Created by defacto on 09.12.16.
//

#ifndef TINTORRENT_TINCONNECTEDSOCKET_H
#define TINTORRENT_TINCONNECTEDSOCKET_H


#include <sys/socket.h>
#include "SocketWrapper.h"
#include "Messages/MessageResourceRequest.h"
#include "Messages/MessageClose.h"
#include "MessageCloseException.h"

using json = nlohmann::json;

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
		checkIfRecievedMessageClose();
	}

	void checkIfRecievedMessageClose(){ // najpierw do jsona i sprawdz typ!!!
		uint8_t  *bufferPtr = buffer.getData();
		if( bufferPtr[0] != 0xff && bufferPtr[1] != 0xff ) //todo from configuration
		{
			json j = getJsonFromBuffer();
			MessageType type;
			type.parseJson(j["Type"]);
			if( type == MessageType::CLOSE){
				MessageClose close = deserializeFromBuffer<MessageClose>();
				throw MessageCloseException(close);
			}
		}
	}

	void sendBuffer(){
		ssize_t sendBytes = send(socket, buffer.getData(), buffer.getSize(), 0);
		auto &varBuffer = buffer;
		Assertions::check([sendBytes, &varBuffer]{return sendBytes == (ssize_t)varBuffer.getSize();}, "Sending failed");
		Assertions::check(sendBytes != 0 , "Has sent 0 bytes, as were in buffer. Propably a mistake as there was 0 in buffer");
	}

	// todo there should be constantId taken into consideration
	template<typename T>
	void serializeToBuffer( T &message ){
		json serializedAsJson = message.toJson();
		std::string jsonDump = serializedAsJson.dump();
		buffer.setData( serializedMessageSizeOffset, (uint8_t *)jsonDump.c_str(), jsonDump.length()+1);

		size_t serializedDataSize = buffer.getSize();
		//todo 4096 from configuration
		Assertions::check(serializedDataSize <= 4096 + serializedMessageSizeOffset, "Serialized data is too big to fit in message");
		*((uint16_t *)buffer.getData()) = (uint16_t)(serializedDataSize);
	}

	template< typename T>
	T deserializeFromBuffer(){
		assertBufferHasSizePrefix();
		json j =  getJsonFromBuffer();
		MessageType expectedType = T::getMessageType();
		MessageType actualType;
		actualType.parseJson(j["Type"]);
		Assertions::check([expectedType, actualType](){ return expectedType == actualType;}, "ExpectedType and actualType dont match"); // tod better error
		return T(j);
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

private:
	json getJsonFromBuffer(){
		Buffer bufferToDeserialize = buffer.getBufferWithOffset(serializedMessageSizeOffset);
		std::string jsonString((const char*) (buffer.getData()+serializedMessageSizeOffset));
		Assertions::check([&jsonString, this](){ return jsonString.length() < buffer.getSize();}, //todo change to equal
		                  "MessageResourceRequest deserialization. InString bigger than buffer");
		// todo throw other exception - something like Model logic exception
		json j = json::parse(jsonString);
		return j;
	}
};


#endif //TINTORRENT_TINCONNECTEDSOCKET_H
