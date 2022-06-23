
#include "client_message.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"
#include "util.hpp"
#include <fstream>
#include <vector>

namespace shared {
	ClientMessage::ClientMessage() { }

	ClientMessage::ClientMessage(ClientMessageData data) {
		mData = data;
	}

	ClientMessageData ClientMessage::getData() {
		return mData;
	}

	void ClientMessage::serialize(BinarySerializer<ClientMessage> *serializer) {
		serializer->writeInt(enumToUnderlying(mData.type));
		switch (mData.type) {
		case ClientMessageType::Created:
		case ClientMessageType::Modified:
			serializer->writeString(mData.filePath);
			writeFile(serializer);
			break;
		case ClientMessageType::Deleted:
		serializer->writeString(mData.filePath);
			break;
		default:
			break;
		}
	}

	void ClientMessage::writeFile(BinarySerializer<ClientMessage> * serializer) {
	    constexpr auto read_size = std::size_t(4096);
		auto stream = std::ifstream(mData.filePath.data());
		stream.exceptions(std::ios_base::badbit);
    
		auto out = std::string();
		auto buf = std::string(read_size, '\0');
		while (stream.read(& buf[0], read_size)) {
			serializer->writeString(buf);
		}
		serializer->writeString(buf);
	}

	void ClientMessage::deserialize(BinaryDeserializer<ClientMessage> *serializer) {
		mData.type = static_cast<ClientMessageType>(serializer->readInt());
		switch (mData.type) {
		default:
			break;
		}
	}
}
