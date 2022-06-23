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
		}

		int getSize() { return mSize; }
		Byte* getData() { return mData; };

		T deserialize() {
			T base;
			base.deserialize(this);
			return base;
		}

		int readInt() {
			if (!canRead(sizeof(int))) {
				std::cerr << "Failed to read integer" << std::endl;
				return -1;
			}

			int i = mData[mCursor];
			mCursor += sizeof(int);
			std::cout << i << std::endl;
			return i;
		}

		std::string readString() {
			int l = readInt();
			std::string s;

			if (!canRead(l)) {
				std::cerr << "Failed to read string" << std::endl;
				return s;
			}
			
			s = std::string(mData + mCursor, mData + mCursor + l);
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
