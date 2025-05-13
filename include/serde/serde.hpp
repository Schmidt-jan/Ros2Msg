#pragma once 

#include <cstdint>
#include <cstddef>

struct SerializedBuffer {
    const uint8_t* data;
    size_t length;
};

class Serde {
public:
    // Template-driven serialize: Returns pointer and length
    template<typename T>
    static T serialize(const T& message, uint8_t* buffer, size_t maxLength) {
        size_t written = message.serialize(buffer, maxLength);
        return { buffer, written };
    }

    // Template-driven deserialize: Returns instance of T
    template<typename T>
    static T deserialize(const uint8_t* buffer, size_t length) {
        T obj;
        if (!obj.deserialize(buffer, length)) {
            // In production embedded systems, you'd handle this safely
            // Here we assume the caller will validate success if needed
        }
        return obj;
    }
};
