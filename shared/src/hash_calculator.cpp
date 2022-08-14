#include "hash_calculator.hpp"
#include "event.hpp"
#include "type.hpp"
#include <bits/c++config.h>
#include <functional>

size_t hash_combine( size_t lhs, size_t rhs ) {
  lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
  return lhs;
}

namespace shared {
    size_t getHash(size_t previousHash, const Event& event) {
	    size_t h1 = previousHash;
        size_t h2 = std::hash<std::string>{}(event.path);
		size_t h3 = std::hash<shared::u64>{}(event.timeModifiedUtcMs);
		size_t h4 = std::hash<int>{}((int)event.type);
		return hash_combine(h1, hash_combine(h2, hash_combine(h3, h4)));
	}
};
