#include "server_message.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"
#include "util.hpp"

namespace shared {
    ServerMessage::ServerMessage(ServerMessageData data) {
        mData = data;
    }

	ServerMessageData ServerMessage::getData() {
		return mData;
	}
	
    void ServerMessage::serialize(BinarySerializer<ServerMessage> *serializer) {
        serializer->writeInt(enumToUnderlying(mData.type));
        switch (mData.type) {
            case ServerMessageType::ResponseStatus: {
                serializer->writeInt(mData.version);
                serializer->writeString(mData.hash);
                break;
            }
            default:
                break;
        }
    }

    void ServerMessage::deserialize(BinaryDeserializer<ServerMessage> *serializer) {
        mData.type = static_cast<ServerMessageType>(serializer->readInt());
        switch (mData.type) {
            case ServerMessageType::ResponseStatus: {
                mData.version = serializer->readInt();
                mData.hash = serializer->readString();
                break;
            }
            default:
                break;
        }
    }

}
