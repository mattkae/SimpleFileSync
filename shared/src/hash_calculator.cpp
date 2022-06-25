#include "hash_calculator.hpp"
#include "event.hpp"
#include <bits/c++config.h>
#include <boost/container_hash/hash_fwd.hpp>
#include <functional>
#include <boost/functional/hash.hpp>

namespace shared {
    size_t getHash(size_t previousHash, const Event& event) {
	    size_t h = 0;
		boost::hash_combine(h, previousHash);
		boost::hash_combine(h, event.path);
		boost::hash_combine(h, event.timeModifiedUtcMs);
		boost::hash_combine(h, event.type);
		return h;
	}
};
