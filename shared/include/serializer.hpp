#pragma once
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include "type.hpp"

namespace shared {
	struct BinarySerializerOptions {
		int bufferSize = 1024;
	};
    
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
			mData = new byte[bso.bufferSize];
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
		byte* getData() { return mData; };

		template<typename S>
		void write(S x) {
			tryGrow(sizeof(S));
			memcpy(&mData[mCurrentSize], &x, sizeof(S));
			mCurrentSize += sizeof(S);
		}
		
		void writeString(std::string& x) {
			write<size_t>(x.length());
			tryGrow(x.length());
			memccpy(&mData[mCurrentSize], x.c_str(), 0, x.length());
			mCurrentSize += x.length();
		}

		template<typename S>
		void writeVector(std::vector<S> list) {
			write<size_t>(list.size());
			for (auto value : list) {
				write(value);
			}
		}

		template <typename S>
		void writeObjectVector(std::vector<S> list) {
			write<size_t>(list.size());
			for (S& value : list) {
				writeObject(value);
			}
		}

		template <typename S>
		void writeObject(S& obj) {
			obj.serialize(*this);
		}

	private:
		byte* mData = NULL;
		int mCurrentSize = 0;
		int mCurrentCapacity = 0;
		void tryGrow(int nextSize) {
		    if (mCurrentSize + nextSize >= mCurrentCapacity) {
				byte* newData = new byte[mCurrentCapacity * 2];
				memccpy(newData, mData, 0, sizeof(byte) * mCurrentSize);
				delete[] mData;
				mData = newData;
				mCurrentCapacity = mCurrentCapacity * 2;
			}
		}
	};
}
