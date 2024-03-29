#pragma once
#include <string>
#include <ctype.h>
#include <vector>
#include "event.hpp"
#include "serializable.hpp"
#include "type.hpp"

namespace shared {
	class BinarySerializer;
	class BinaryDeserializer;
	
	enum class ClientMessageType: shared::i16 {
		None = 0,
		RequestStartComm,
		RequestEndComm,
		ChangeEvent,
		RequestResolution
	};

	class ClientMessage: public ISerializable {
	public:
		ClientMessage();
		void serialize(BinarySerializer& serializer) override;
		void deserialize(BinaryDeserializer& serializer) override;

		ClientMessageType type;
		Event event;
		std::vector<shared::u64> hashList;
	};
}
