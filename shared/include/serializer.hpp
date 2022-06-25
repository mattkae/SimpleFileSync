#pragma once
#include <cstring>
#include <string>
#include <iostream>

namespace shared {
	typedef char Byte;

	struct BinarySerializerOptions {
		int bufferSize = 1024;
	};
	
	template <typename T>
	class BinarySerializer {
	public:
		BinarySerializer() {
			init(BinarySerializerOptions());
		}
		
		BinarySerializer(BinarySerializerOptions bso) {
			init(bso);
		}

		void init(BinarySerializerOptions bso) {
			if (mData) {
				free();
			}
			mData = new Byte[bso.bufferSize];
			mCurrentCapacity = bso.bufferSize;
			mCurrentSize = 0;
		}

		~BinarySerializer() {
			free();
		}

		void free() {
			if (mData) delete[] mData;
			mData = NULL;
			mCurrentCapacity = 0;
			mCurrentSize = 0;
		}

		void reset() {
			mCurrentSize = 0;
		}

		int getSize() { return mCurrentSize; }
		Byte* getData() { return mData; };

		void serialize(T value) {
			value.serialize(this);
		}

		T deserialize() {
			T base;
			base.deserialize(this);
			return base;
		}

		template<typename S>
		void write(S x) {
			tryGrow(sizeof(S));
			memccpy(&mData[mCurrentSize], &x, 0, sizeof(S));
			mCurrentSize += sizeof(S);
		}
		
		void writeString(std::string x) {
			int strSize = sizeof(char) * x.length();
			write(strSize);
			tryGrow(strSize);
			memccpy(&mData[mCurrentSize], x.c_str(), 0, strSize);
			mCurrentSize += strSize;
		}

	private:
		Byte* mData = NULL;
		int mCurrentSize = 0;
		int mCurrentCapacity = 0;
		void tryGrow(int nextSize) {
		    if (mCurrentSize + nextSize >= mCurrentCapacity) {
				Byte* newData = new Byte[mCurrentCapacity * 2];
				memccpy(newData, mData, 0, sizeof(Byte) * mCurrentSize);
				delete[] mData;
				mData = newData;
				mCurrentCapacity = mCurrentCapacity * 2;
			}
		}
	};
}
