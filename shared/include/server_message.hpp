#pragma once
#include "event.hpp"
#include "serializable.hpp"
#include <cstddef>
#include <string>
#include <ctype.h>
#include <vector>

namespace shared {
	class BinarySerializer;
	class BinaryDeserializer;
	
	enum class ServerMessageType {
		None = 0,
		ResponseStartComm,
		ReponseEndComm,
		Created,
		Deleted,
		Modified
	};

	struct ServerMessageData {
		ServerMessageType type;
		std::string filePath = "";
		std::vector<Event> eventsForClient;
	};

	class ServerMessage : public ISerializable {
	public:
		ServerMessage() { }
		ServerMessage(ServerMessageData data);
		void serialize(BinarySerializer& serializer);
		void deserialize(BinaryDeserializer& serializer);
		ServerMessageData getData();
	private:
	    ServerMessageData mData;
	};
	
}
