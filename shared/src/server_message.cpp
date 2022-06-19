#include "server_message.hpp"
#include "serializer.hpp"
#include "util.hpp"

namespace shared {
    ServerMessage::ServerMessage(ServerMessageData data) {
        mData = data;
    }

    void ServerMessage::serialize(BinarySerializer<ServerMessage> *serializer) {
        serializer->serializeInt(enumToUnderlying(mData.type));
    }
}