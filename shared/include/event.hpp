#pragma once
#include <string>
#include "serializer.hpp"
#include "deserializer.hpp"

namespace shared {
	enum class EventType {
		None = 0,
		Created,
		Deleted,
		Modified
	};

	struct Event {
		EventType type;
		std::string fullpath; // @Warning: Not serialized
		std::string path;
		long timeModifiedUtcMs;
		std::string content;

		void serialize(BinarySerializer<Event>* serializer);
		void deserialize(BinaryDeserializer<Event>* serializer);
	};

	bool executeEvent(const Event& event, const std::string&& mDirectory);
	bool recordEvent(const Event& event, size_t hash);
	Event retrieveEvent(size_t hash);
}
