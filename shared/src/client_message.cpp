
#include "client_message.hpp"
#include "event.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"

namespace shared {
	ClientMessage::ClientMessage() { }

	void ClientMessage::serialize(BinarySerializer& serializer) {
		serializer.write<shared::i8>(getEnumType(type));
		switch (type) {
		case ClientMessageType::RequestStartComm: {
			serializer.write(event.hash);
			break;
		}
		case ClientMessageType::ChangeEvent:
			serializer.writeObject(event);
			break;
		default:
			break;
		}
	}

	void ClientMessage::deserialize(BinaryDeserializer& serializer) {
		type = static_cast<ClientMessageType>(serializer.read<shared::i8>());
		switch (type) {
		case ClientMessageType::RequestStartComm: {
			event.hash = serializer.read<shared::u64>();
			break;
		}
		case ClientMessageType::ChangeEvent: {
			event = serializer.readObject<Event>();
			break;
		}
		default:
			break;
		}
	}
}
