
#pragma once

namespace shared {
    class BinarySerializer;
    class BinaryDeserializer;

    class ISerializable {
        virtual void serialize(BinarySerializer& serializer) = 0;
        virtual void deserialize(BinaryDeserializer& serializer) = 0;
    };
}
