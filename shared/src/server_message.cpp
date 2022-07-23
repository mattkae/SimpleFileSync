#include "server_message.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"
#include "util.hpp"
#include <cstddef>

namespace shared {
    ServerMessage::ServerMessage(ServerMessageData data) {
        mData = data;
    }

	ServerMessageData ServerMessage::getData() {
		return mData;
	}
	
    void ServerMessage::serialize(BinarySerializer<ServerMessage> *serializer) {
        serializer->write(enumToUnderlying(mData.type));
        switch (mData.type) {
            case ServerMessageType::ResponseAskClientToResolve: {
                serializer->write(mData.hashList.size());
                for (auto hash : mData.hashList) {
                    serializer->write(hash);
                }
                break;
            }
            case ServerMessageType::ResponseStartComm:
            default:
                break;
        }
    }

    void ServerMessage::deserialize(BinaryDeserializer<ServerMessage> *serializer) {
        mData.type = static_cast<ServerMessageType>(serializer->read<int>());
        switch (mData.type) {
            case ServerMessageType::ResponseAskClientToResolve: {
                size_t numElements = serializer->read<size_t>();
                for (size_t i = 0; i < numElements; i++) {
                    mData.hashList.push_back(serializer->read<size_t>());
                }
                break;
            }
            case ServerMessageType::ResponseStartComm:
            default:
                break;
        }
    }

}
