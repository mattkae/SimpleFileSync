#pragma once
#include <string>

namespace shared {

	template <typename T>
	class BinarySerializer;

	template <typename T>
	class BinaryDeserializer;
	
	enum class ClientMessageType {
		None = 0,
		RequestStatus,
		RequestEndComm,
		Created,
		Deleted,
		Modified,
	};

	struct ClientMessageData {
		ClientMessageType type;
		std::string filePath = "";
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
