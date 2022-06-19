#pragma once
#include <string>

namespace shared {

	template <typename T>
	class BinarySerializer;
	
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
		ClientMessage(ClientMessageData data);
		void serialize(BinarySerializer<ClientMessage>* serializer);
		void deserialize(BinarySerializer<ClientMessage>* serializer);
	private:
		ClientMessageData mData;
		void writeFile(BinarySerializer<ClientMessage>* serializer);
		
	};

}
