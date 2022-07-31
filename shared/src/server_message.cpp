#include "server_message.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"
#include <cstddef>

namespace shared {
    void ServerMessage::serialize(BinarySerializer& serializer) {
        serializer.write(getEnumType(type));
        switch (type) {
            case ServerMessageType::ResponseStartComm:
                serializer.writeObjectVector(eventsForClient);
                break;
            default:
                break;
        }
    }

    void ServerMessage::deserialize(BinaryDeserializer& deserializer) {
        type = static_cast<ServerMessageType>(deserializer.read<shared::i8>());
        switch (type) {
            case ServerMessageType::ResponseStartComm:
                eventsForClient = deserializer.readObjectVector<Event>();
                break;
            default:
                break;
        }
    }

}
