
#include "client_message.hpp"
#include "serializer.hpp"
#include "util.hpp"
#include <fstream>
#include <vector>

namespace shared {

	ClientMessage::ClientMessage(ClientMessageData data) {
		mData = data;
	}

	void ClientMessage::serialize(BinarySerializer<ClientMessage> *serializer) {
		serializer->serializeInt(enumToUnderlying(mData.type));
		switch (mData.type) {
		case ClientMessageType::Created:
		case ClientMessageType::Modified:
			serializer->serializeString(mData.filePath);
			writeFile(serializer);
			break;
		case ClientMessageType::Deleted:
		serializer->serializeString(mData.filePath);
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
			serializer->serializeString(buf);
		}
		serializer->serializeString(buf);
	}
}
