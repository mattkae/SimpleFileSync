#pragma once
#include <string>

namespace shared {

	template <typename T>
	class BinarySerializer;
	
	enum class ClientMessageType {
		None = 0,
		Created,
		Deleted,
		Modified
	};

	class ClientMessage {
	public:
		ClientMessage(ClientMessageType type, std::string filePath);
		void serialize(BinarySerializer<ClientMessage>* serializer);
		void deserialize(BinarySerializer<ClientMessage>* serializer);
	private:
		std::string mFilePath;
		ClientMessageType mType;
		void writeFile(BinarySerializer<ClientMessage>* serializer);
		
	};

}
