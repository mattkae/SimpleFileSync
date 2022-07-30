#pragma once
#include <string>
#include "serializer.hpp"
#include "deserializer.hpp"
#include "stringable.hpp"
#include "serializable.hpp"

namespace shared {
	enum class EventType {
		None = 0,
		Created,
		Deleted,
		Modified
	};

	struct Event : public IStringable, public ISerializable {
		size_t hash;		  // @Warning: Not serialized
		EventType type;
		std::string fullpath; // @Warning: Not serialized
		std::string path;
		long timeModifiedUtcMs;
		std::string content;

		void serialize(BinarySerializer& serializer);
		void deserialize(BinaryDeserializer& serializer);
		std::string toString() const override;
	};

	bool executeEvent(const Event& event, const std::string&& mDirectory);
}
