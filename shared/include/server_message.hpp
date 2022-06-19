#pragma once
#include <string>

namespace shared {

	template <typename T>
	class BinarySerializer;
	
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
		std::string filePath = "";
	};

	class ServerMessage {
	public:
		ServerMessage(ServerMessageData data);
		void serialize(BinarySerializer<ServerMessage>* serializer);
		void deserialize(BinarySerializer<ServerMessage>* serializer);
	private:
	    ServerMessageData mData;
	};
	
}
