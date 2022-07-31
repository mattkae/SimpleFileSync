#pragma once
#include <cstddef>
#include <cstring>
#include <ostream>
#include <string>
#include <iostream>
#include <spdlog/spdlog.h>
#include <vector>
#include "type.hpp"

namespace shared {
	struct BinaryDeserializerOptions {
        byte* data = NULL;
		size_t dataSize = 0;
		size_t cursorOffset = 0;
	};
	
	class BinaryDeserializer {
	public:
		BinaryDeserializer(BinaryDeserializerOptions bso) {
			init(bso);
		}

		void init(BinaryDeserializerOptions bso) {
			mSize = bso.dataSize;
            mData = bso.data;
			mCursor = bso.cursorOffset;
		}

		size_t getSize() { return mSize; }
		size_t getCursor() { return mCursor; }
		byte* getData() { return mData; };

		template<typename S>
		S read() {
			if (!canRead(sizeof(S))) {
				spdlog::error("Failed to read type, type=", typeid(S).name());
				return -1;
			}

			S s;
			memcpy(&s, &mData[mCursor], sizeof(S));
			mCursor += sizeof(S);
			return s;
		}

		std::string readString() {
			size_t l = read<size_t>();
			std::string s;

			if (!canRead(l)) {
				spdlog::error("Failed to read type, type=string");
				return s;
			}
			
			auto start = mCursor;
			auto end = start + l;
			s = std::string((char*)mData + start, end - start);
			mCursor += l;
			return s;
		}

		template<typename S>
		std::vector<S> readVector() {
			size_t l = read<size_t>();
			std::vector<S> retval(l);
			for (size_t i = 0; i < l; i++) {
				retval.push_back(read<S>());
			}

			return retval;
		}

		template<typename S>
		std::vector<S> readObjectVector() {
			size_t l = read<size_t>();
			std::vector<S> retval;
			retval.reserve(1);
			for (size_t i = 0; i < l; i++) {
				retval.push_back(readObject<S>());
			}

			return retval;
		}

		template<typename S>
		S readObject() {
			S defaultS;
			defaultS.deserialize(*this);
			return defaultS;
		}

	private:
		byte* mData = NULL;
		size_t mCursor = 0;
		size_t mSize = 0;

		bool canRead(int increment) {
			return (mCursor + increment) <= mSize;
		}
	};
}
