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

	void readToBuffer();

	void checkIfRecievedMessageClose();

	void sendBuffer();

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

	void assertBufferHasGoodConstantPrefix();

	void assertBufferHasGoodSizePrefix();

	bool constantPrefixIsFromSegmentResponse();

public:
	TinConnectedSocket( socket_descriptor_t socket );;

	void closeConnection( MessageClose::CloseReason closeReason );

private:
	json getJsonFromBuffer();

	uint16_t  getConstantPrefixFromBuffer();

	uint16_t  getSizeFromBuffer();

};


#endif //TINTORRENT_TINCONNECTEDSOCKET_H
