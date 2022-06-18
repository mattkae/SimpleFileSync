
#include "message.hpp"
#include "serializer.hpp"
#include <type_traits>
#include <fstream>
#include <vector>

namespace shared {

	ClientMessage::ClientMessage(ClientMessageType type, std::string filePath) {
		mType = type;
		mFilePath = filePath;
	}


	template <typename E>
	constexpr auto toUnderlying(E e) noexcept {
		return static_cast<std::underlying_type_t<E>>(e);
	}

	void ClientMessage::serialize(BinarySerializer<ClientMessage> *serializer) {
		serializer->serializeInt(toUnderlying(mType));
		serializer->serializeString(mFilePath);
		switch (mType) {
		case ClientMessageType::Created:
		case ClientMessageType::Modified:
			writeFile(serializer);
			break;
		default:
			break;
		}
	}

	void ClientMessage::writeFile(BinarySerializer<ClientMessage> * serializer) {
	    constexpr auto read_size = std::size_t(4096);
		auto stream = std::ifstream(mFilePath.data());
		stream.exceptions(std::ios_base::badbit);
    
		auto out = std::string();
		auto buf = std::string(read_size, '\0');
		while (stream.read(& buf[0], read_size)) {
			serializer->serializeString(buf);
		}
		serializer->serializeString(buf);
	}
}
