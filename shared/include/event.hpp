#pragma once
#include <string>

namespace shared {
	enum class EventType {
		None = 0,
		Created,
		Deleted,
		Modified
	};

	struct Event {
		EventType type;
		std::string path;
		long timeModifiedUtcMs;
	};

	bool executeEvent(const Event& event, const std::string&& mDirectory);
}
