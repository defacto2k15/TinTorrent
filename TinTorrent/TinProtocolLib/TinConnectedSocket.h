//
// Created by defacto on 09.12.16.
//

#ifndef TINTORRENT_TINCONNECTEDSOCKET_H
#define TINTORRENT_TINCONNECTEDSOCKET_H


#include <sys/socket.h>
#include <TinProtocolLib/Messages/SegmentResponse.h>
#include "SocketWrapper.h"
#include "Messages/MessageResourceRequest.h"
#include "Messages/MessageClose.h"
#include "MessageCloseException.h"
#include "SocketCommunicationException.h"

using json = nlohmann::json;

class TinConnectedSocket : public SocketWrapper {
	socket_descriptor_t  socket;
	// todo from conf
	const size_t serializedMessageSizeOffset = 4;
protected:
	InMemoryBuffer buffer;

	void readToBuffer(){
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

	void checkIfRecievedMessageClose(){
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

	void sendBuffer(){
		ssize_t sendBytes = send(socket, buffer.getData(), buffer.getSize(), 0);
		auto &varBuffer = buffer;
		Assertions::check<SocketCommunicationException>([sendBytes, &varBuffer]{return sendBytes == (ssize_t)varBuffer.getSize();}, "Sending failed");
		Assertions::check<SocketCommunicationException>(sendBytes != 0 , "Has sent 0 bytes, as were in buffer. Propably a mistake as there was 0 in buffer");
	}

	// todo there should be constantId taken into consideration
	template<typename T>
	void serializeToBuffer( T &message ){
		json serializedAsJson = message.toJson();
		std::string jsonDump = serializedAsJson.dump();
		buffer.setData( serializedMessageSizeOffset, (uint8_t *)jsonDump.c_str(), jsonDump.length()+1);
		size_t serializedDataSize = buffer.getSize();
		uint8_t	*dataPtr = buffer.getData();
		dataPtr[0] = 0xff;
		dataPtr[1] = 0xfd;
		*((uint16_t*)(dataPtr + 2)) = (uint16_t )serializedDataSize;

		Assertions::check(serializedDataSize <= Constants::segmentSize + serializedMessageSizeOffset, "Serialized data is too big to fit in message");
	}

	template< typename T>
	T deserializeFromBuffer(){
		assertBufferHasGoodConstantPrefix();
		assertBufferHasGoodSizePrefix();
		json j =  getJsonFromBuffer();
		MessageType expectedType = T::getMessageType();
		MessageType actualType;
		actualType.parseJson(j["Type"]);
		Assertions::check([expectedType, actualType](){ return expectedType == actualType;}, "ExpectedType and actualType dont match"); // tod better error
		return T(j);
	}

	void assertBufferHasGoodConstantPrefix(){
		Assertions::check<SocketCommunicationException>( buffer.getData()[0] == 0xff && (buffer.getData()[1] == 0xfd || buffer.getData()[1] == 0xff),
					Help::Str("Received data has bad constants prefix. First 4bytes are ", *((uint32_t*)buffer.getData())));
	}

	void assertBufferHasGoodSizePrefix(){
		uint16_t sizePrefix = *((uint16_t *)(buffer.getData()+2));
		Assertions::check<SocketCommunicationException>( sizePrefix == buffer.getSize(),
		   Help::Str("SizePrefix ", sizePrefix, "is not equal to buffer content length ",buffer.getSize(),
		             ". First 4 bytes are ", *((uint32_t*)buffer.getData())));
	}

	bool constantPrefixIsFromSegmentResponse(){
		return buffer.getData()[0] == 0xff && buffer.getData()[1] == 0xff;
	}

public:
	TinConnectedSocket( socket_descriptor_t socket ) : socket(socket ), buffer( Constants::segmentSize+300){};

	void closeConnection( MessageClose::CloseReason closeReason ){
		MessageClose messageClose(closeReason);
		serializeToBuffer(messageClose);
		sendBuffer();
	}

private:
	json getJsonFromBuffer(){
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

	uint16_t  getConstantPrefixFromBuffer(){
		return *(uint16_t*)buffer.getData();
	}

	uint16_t  getSizeFromBuffer(){
		return *(uint16_t*)(buffer.getData()+2);
	}

};


#endif //TINTORRENT_TINCONNECTEDSOCKET_H
