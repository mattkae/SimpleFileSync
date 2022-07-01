#pragma once
#include <cstddef>
#include <cstring>
#include <ostream>
#include <string>
#include <iostream>

namespace shared {
	typedef char Byte;

	struct BinaryDeserializerOptions {
        Byte* data = NULL;
		size_t dataSize = 0;
		size_t cursorOffset = 0;
	};
	
	template <typename T>
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
		Byte* getData() { return mData; };

		size_t deserialize(T& out) {
			out.deserialize(this);
			return mCursor;
		}

		template<typename S>
		S read() {
			if (!canRead(sizeof(S))) {
				std::cerr << "Failed to read type" << std::endl;
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
				std::cerr << "Failed to read string" << std::endl;
				return s;
			}
			
			auto start = mCursor;
			auto end = start + l;
			s = std::string(mData + start, end - start);
			mCursor += l;
			return s;
		}

	private:
		Byte* mData = NULL;
		size_t mCursor = 0;
		size_t mSize = 0;

		bool canRead(int increment) {
			return (mCursor + increment) <= mSize;
		}
	};
}
