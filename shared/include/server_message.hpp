#pragma once
#include "event.hpp"
#include <cstddef>
#include <string>
#include <ctype.h>
#include <vector>

namespace shared {

	template <typename T>
	class BinarySerializer;

	template <typename T>
	class BinaryDeserializer;
	
	enum class ServerMessageType {
		None = 0,
		ResponseAskClientToResolve,
		ResponseAskClientForUpdate,
		ResponseStartComm,
		ReponseEndComm,
		Created,
		Deleted,
		Modified
	};

	struct ServerMessageData {
		ServerMessageType type;
		std::string filePath = "";
		std::vector<shared::Event> eventsToUpdate;
		std::vector<size_t> hashList;
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
