#pragma once
#include <cstring>

namespace shared {
	struct Event;
	
    size_t getHash(size_t previousHash, const Event& event);
};
