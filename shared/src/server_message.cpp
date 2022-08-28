#include "server_message.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"
#include "type.hpp"
#include <cstddef>

namespace shared {
    ServerMessage::ServerMessage() { }
    ServerMessage::~ServerMessage() { }

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
        type = static_cast<ServerMessageType>(deserializer.readi16());
        switch (type) {
            case ServerMessageType::ResponseStartComm:
                eventsForClient = deserializer.readObjectVector<Event>();
                break;
            default:
                break;
        }
    }

}
