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
	
	enum class ServerMessageType: shared::i16 {
		None = 0,
		ResponseStartComm,
		ReponseEndComm,
		Created,
		Deleted,
		Modified
	};

	class ServerMessage : public ISerializable {
	public:
		ServerMessage();
		~ServerMessage();
		void serialize(BinarySerializer& serializer) override;
		void deserialize(BinaryDeserializer& serializer) override;

		ServerMessageType type;
		std::string filePath = "";
		std::vector<Event> eventsForClient;
	};
	
}
