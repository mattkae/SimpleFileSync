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
	
	enum class ClientMessageType: shared::i8 {
		None = 0,
		RequestStartComm,
		RequestEndComm,
		ChangeEvent,
		RequestResolution
	};

	class ClientMessage: public ISerializable {
	public:
		ClientMessage();
		void serialize(BinarySerializer& serializer);
		void deserialize(BinaryDeserializer& serializer);

		ClientMessageType type;
		Event event;
		std::vector<shared::u64> hashList;
	};
}
