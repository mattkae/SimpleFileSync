#pragma once
#include <cstring>
#include <string>

namespace shared {
	typedef unsigned char Byte;

	struct BinarySerializerOptions {
		int bufferSize = 1024;
	};
	
	template <typename T>
	class BinarySerializer {
	public:
		BinarySerializer() { }
		
		BinarySerializer(BinarySerializerOptions bso) {
			mData = new Byte[bso.bufferSize];
			mCurrentCapacity = bso.bufferSize;
			mCurrentSize = 0;
		}

		~BinarySerializer() {
			delete[] mData;
			mCurrentCapacity = 0;
			mCurrentSize = 0;
		}

		void serialize(const T&& value) {
			value.serialize(this);
		}

		T deserialize() {
			T base;
			base.deserialize(this);
			return base;
		}

		void serializeInt(int x) {
			tryGrow(sizeof(int));
			memccpy(&mData[mCurrentSize], &x, 0, sizeof(int));
			mCurrentSize += sizeof(int);
		}
		
		void serializeString(std::string x) {
			int strSize = sizeof(char) * x.length();
			tryGrow(strSize);
			memccpy(&mData[mCurrentSize], &strSize, 0, sizeof(int));
			mCurrentSize += sizeof(int);
			memccpy(&mData[mCurrentSize], x.c_str(), 0, sizeof(char) * x.length());
			mCurrentSize += strSize;
		}

	private:
		Byte* mData;
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
