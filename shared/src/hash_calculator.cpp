#include "hash_calculator.hpp"
#include "event.hpp"
#include <bits/c++config.h>
#include <functional>

namespace shared {
    size_t getHash(size_t previousHash, const Event& event) {
	    size_t h = 0;
		// boost::hash_combine(h, previousHash);
		// boost::hash_combine(h, event.path);
		// boost::hash_combine(h, event.timeModifiedUtcMs);
		// boost::hash_combine(h, event.type);
		return h;
	}
};
