#pragma once
#include <string>
#include "serializer.hpp"
#include "deserializer.hpp"
#include "stringable.hpp"
#include "serializable.hpp"
#include "type.hpp"

namespace shared {
	enum class EventType: shared::i16 {
		None = 0,
		Created,
		Deleted,
		Modified
	};

	class Event : public IStringable, public ISerializable {
	public:
		virtual ~Event() = default; // See: https://stackoverflow.com/questions/10024796/c-virtual-functions-but-no-virtual-destructors
		shared::u64 hash;
		EventType type;
		std::string fullpath; // @Warning: Not serialized
		std::string path;
		shared::u64 timeModifiedUtcMs;
		std::string content;

		void serialize(BinarySerializer& serializer) override;
		void deserialize(BinaryDeserializer& serializer) override;
		std::string toString() const override;
	};

	bool executeEvent(const Event& event, const std::string&& mDirectory);
}
