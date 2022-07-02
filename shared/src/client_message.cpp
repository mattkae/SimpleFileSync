
#include "client_message.hpp"
#include "event.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"
#include "util.hpp"
#include <fstream>
#include <iterator>
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
		serializer->write<int>(enumToUnderlying(mData.type));
		switch (mData.type) {
		case ClientMessageType::RequestStartComm: {
			serializer->write<size_t>(mData.numberOfMessages);
			break;
		}
		case ClientMessageType::ChangeEvent:
			serializer->write<int>(enumToUnderlying(mData.event.type));
			serializer->write<size_t>(mData.hash);
			serializer->write(mData.event.timeModifiedUtcMs);
			serializer->writeString(mData.event.path);

			if (mData.event.type == shared::EventType::Created || mData.event.type == shared::EventType::Modified) {
				writeFile(serializer);
			}
			break;
		default:
			break;
		}
	}

	void ClientMessage::writeFile(BinarySerializer<ClientMessage> * serializer) {
		// @TODO Investigate efficiency as files get larger
	    constexpr auto read_size = std::size_t(4096);
		auto stream = std::ifstream(mData.event.fullpath);
		stream.exceptions(std::ios_base::badbit);
    
		auto out = std::string();
		auto buf = std::string(read_size, '\0');
		while (stream.read(& buf[0], read_size)) {
			out.append(buf, 0, stream.gcount());
		}
		out.append(buf, 0, stream.gcount());

		serializer->writeString(out);
	}

	void ClientMessage::deserialize(BinaryDeserializer<ClientMessage> *serializer) {
		mData.type = static_cast<ClientMessageType>(serializer->read<int>());
		switch (mData.type) {
		case ClientMessageType::RequestStartComm: {
			mData.numberOfMessages = serializer->read<size_t>();
			break;
		}
		case ClientMessageType::ChangeEvent: {
			mData.event.type = (EventType)serializer->read<int>();
			mData.hash = serializer->read<size_t>();
			mData.event.timeModifiedUtcMs = serializer->read<long>();
			mData.event.path = serializer->readString();

			if (mData.event.type == shared::EventType::Created || mData.event.type == shared::EventType::Modified) {
				mData.event.content = serializer->readString();
			}

			break;
		}
		default:
			break;
		}
	}
}
