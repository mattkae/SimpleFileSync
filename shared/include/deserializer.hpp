#pragma once
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

		int getSize() { return mSize; }
		Byte* getData() { return mData; };

		size_t deserialize(T& out) {
			out.deserialize(this);
			return mCursor;
		}

		template<typename S>
		S read() {
			if (!canRead(sizeof(S))) {
				std::cerr << "Failed to read integer" << std::endl;
				return -1;
			}

			S s = mData[mCursor];
			mCursor += sizeof(int);
			return s;
		}

		std::string readString() {
			int l = read<int>();
			std::string s;

			if (!canRead(l)) {
				std::cerr << "Failed to read string" << std::endl;
				return s;
			}
			
			auto start = mCursor;
			auto end = start + l;
			s = std::string(mData + start, end - start);
			std::cout << s << std::endl;
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
