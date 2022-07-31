#pragma once
#include "event.hpp"
#include "serializable.hpp"
#include <cstddef>
#include <string>
#include <ctype.h>
#include <vector>
#include "type.hpp"

namespace shared {
	class BinarySerializer;
	class BinaryDeserializer;
	
	enum class ServerMessageType: shared::i8 {
		None = 0,
		ResponseStartComm,
		ReponseEndComm,
		Created,
		Deleted,
		Modified
	};

	class ServerMessage : public ISerializable {
	public:
		ServerMessage() { }
		void serialize(BinarySerializer& serializer);
		void deserialize(BinaryDeserializer& serializer);

		ServerMessageType type;
		std::string filePath = "";
		std::vector<Event> eventsForClient;
	};
	
}
