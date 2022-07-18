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
        serializer->write(enumToUnderlying(mData.type));
        switch (mData.type) {
            case ServerMessageType::ResponseStartComm:
            default:
                break;
        }
    }

    void ServerMessage::deserialize(BinaryDeserializer<ServerMessage> *serializer) {
        mData.type = static_cast<ServerMessageType>(serializer->read<int>());
        switch (mData.type) {
            case ServerMessageType::ResponseStartComm:
            default:
                break;
        }
    }

}
