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
	
    void ServerMessage::serialize(BinarySerializer& serializer) {
        serializer.write(enumToUnderlying(mData.type));
        switch (mData.type) {
            case ServerMessageType::ResponseStartComm:
                serializer.writeObjectVector(mData.eventsForClient);
                break;
            default:
                break;
        }
    }

    void ServerMessage::deserialize(BinaryDeserializer& deserializer) {
        mData.type = static_cast<ServerMessageType>(deserializer.read<int>());
        switch (mData.type) {
            case ServerMessageType::ResponseStartComm:
                mData.eventsForClient = deserializer.readObjectVector<Event>();
                break;
            default:
                break;
        }
    }

}
