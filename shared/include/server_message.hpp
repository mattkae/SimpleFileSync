#pragma once
#include <string>

namespace shared {

	template <typename T>
	class BinarySerializer;

	template <typename T>
	class BinaryDeserializer;
	
	enum class ServerMessageType {
		None = 0,
		ResponseStatus,
		ReponseEndComm,
		Created,
		Deleted,
		Modified
	};

	struct ServerMessageData {
		ServerMessageType type;
		int version = 1;
		std::string hash = "abc";
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
