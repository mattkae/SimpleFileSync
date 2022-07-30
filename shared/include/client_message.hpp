#pragma once
#include <string>
#include <ctype.h>
#include <vector>
#include "event.hpp"
#include "serializable.hpp"

namespace shared {
	class BinarySerializer;
	class BinaryDeserializer;
	
	enum class ClientMessageType {
		None = 0,
		RequestStartComm,
		RequestEndComm,
		ChangeEvent,
		RequestResolution
	};

	struct ClientMessageData {
		ClientMessageType type;
	    size_t hash;
		Event event;
		std::vector<size_t> hashList;
	};

	class ClientMessage: public ISerializable {
	public:
		ClientMessage();
		ClientMessage(ClientMessageData data);
		void serialize(BinarySerializer& serializer);
		void deserialize(BinaryDeserializer& serializer);
		ClientMessageData getData();
	private:
		ClientMessageData mData;
		void writeFile(BinarySerializer& serializer);
		
	};

}
