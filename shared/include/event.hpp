#pragma once
#include <string>
#include "serializer.hpp"
#include "deserializer.hpp"
#include "stringable.hpp"
#include "serializable.hpp"
#include "type.hpp"

namespace shared {
	enum class EventType {
		None = 0,
		Created,
		Deleted,
		Modified
	};

	struct Event : public IStringable, public ISerializable {
		Event();
		~Event();
		shared::u64 hash;	// @Warning: Not serialized
		EventType type;
		std::string fullpath; // @Warning: Not serialized
		std::string path;
		shared::u64 timeModifiedUtcMs;
		std::string content;

		void serialize(BinarySerializer& serializer);
		void deserialize(BinaryDeserializer& serializer);
		std::string toString() const override;
	};

	bool executeEvent(const Event& event, const std::string&& mDirectory);
}
