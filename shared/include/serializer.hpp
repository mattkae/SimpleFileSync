#pragma once
#include <cstring>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include "type.hpp"

#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))

namespace shared {
	struct BinarySerializerOptions {
		i32 bufferSize = 1024;
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

		i32 getSize() { return mCurrentSize; }
		byte* getData() { return mData; };

		template<typename S>
		void _writeInternal(S x) {
			tryGrow(sizeof(S));
			memcpy(&mData[mCurrentSize], &x, sizeof(S));
			mCurrentSize += sizeof(S);
		}

		void write(shared::u64 x) {
			_writeInternal(htonll(x));
		}

		void write(shared::u32 x) {
			_writeInternal(htonl(x));
		}

		void write(shared::i32 x) {
			_writeInternal(htonl(x));
		}

		void write(shared::i16 x) {
			_writeInternal(htons(x));
		}

		void write(shared::i8 x) {
			_writeInternal(htons(x));
		}
		
		void writeString(std::string& x) {
			u64 l = x.length();
			write(l);
			tryGrow(l);
			memccpy(&mData[mCurrentSize], x.c_str(), 0, x.length());
			mCurrentSize += x.length();
		}

		template<typename S>
		void writeVector(std::vector<S> list) {
			write(list.size());
			for (auto value : list) {
				write(value);
			}
		}

		template <typename S>
		void writeObjectVector(std::vector<S> list) {
			write(list.size());
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
		i32 mCurrentSize = 0;
		i32 mCurrentCapacity = 0;
		void tryGrow(i32 nextSize) {
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
