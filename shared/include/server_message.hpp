#pragma once
#include <cstddef>
#include <string>
#include <ctype.h>

namespace shared {

	template <typename T>
	class BinarySerializer;

	template <typename T>
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
		int version = 1;
		size_t hash = 0;
		std::string filePath = "";
	};

	class ServerMessage {
	public:
		ServerMessage() { }
		ServerMessage(ServerMessageData data);
		void serialize(BinarySerializer<ServerMessage>* serializer);
		void deserialize(BinaryDeserializer<ServerMessage>* serializer);
		ServerMessageData getData();
	private:
	    ServerMessageData mData;
	};
	
}
