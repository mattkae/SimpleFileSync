#pragma once
#include <string>
#include <ctype.h>
#include "event.hpp"

namespace shared {

	template <typename T>
	class BinarySerializer;

	template <typename T>
	class BinaryDeserializer;
	
	enum class ClientMessageType {
		None = 0,
		RequestStartComm,
		RequestEndComm,
		ChangeEvent
	};
	

	struct ClientMessageData {
		ClientMessageType type;
	    size_t hash;
		Event event;
	};

	class ClientMessage {
	public:
		ClientMessage();
		ClientMessage(ClientMessageData data);
		void serialize(BinarySerializer<ClientMessage>* serializer);
		void deserialize(BinaryDeserializer<ClientMessage>* serializer);
		ClientMessageData getData();
	private:
		ClientMessageData mData;
		void writeFile(BinarySerializer<ClientMessage>* serializer);
		
	};

}
