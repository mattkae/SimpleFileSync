#pragma once
#include <cstddef>
#include <cstring>
#include <exception>
#include <netinet/in.h>
#include <ostream>
#include <string>
#include <iostream>
#include <vector>
#include "logger.hpp"
#include "type.hpp"
#include <arpa/inet.h>

#define ntohll(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))

namespace shared {
	struct BinaryDeserializerOptions {
        byte* data = NULL;
		u64 dataSize = 0;
		u64 cursorOffset = 0;
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

		u64 getSize() { return mSize; }
		u64 getCursor() { return mCursor; }
		byte* getData() { return mData; };

		template<typename S>
		S _readInternal(const char* debugMessage) {
			S s = 0;
			if (!canRead(sizeof(S))) {
				logger_error("Failed to read value: %s -> currentSize: %lu, cursor: %lu, desiredSize: %lu", debugMessage, getSize(), getCursor(), getSize() + sizeof(S));
				return s;
			}

			memcpy(&s, &mData[mCursor], sizeof(S));
			mCursor += sizeof(S);
			return  s;
		}

		u64 readu64() {
			u64 value = _readInternal<u64>("u64");
			return ntohll(value);
		}

		u32 readu32() {
			return ntohl(_readInternal<u32>("u32"));
		}

		i32 readi32() {
			return ntohl(_readInternal<i32>("i32"));
		}

		i16 readi16() {
			return ntohs(_readInternal<i16>("i16"));
		}

		i8 readi8() {
			return ntohs(_readInternal<i8>("i8"));
		}

		std::string readString() {
			u64 l = readu64();
			std::string s;

			if (!canRead(l)) {
				logger_error("Failed to read string value, size=%lu", l);
				return s;
			}
			
			auto start = mCursor;
			auto end = start + l;
			s = std::string((char*)mData + start, end - start);
			mCursor += l;
			return s;
		}

		template<typename S>
		std::vector<S> readObjectVector() {
			u64 l = readu64();
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
		u64 mCursor = 0;
		u64 mSize = 0;

		bool canRead(u64 increment) {
			return (getCursor() + increment) <= getSize();
		}
	};
}
