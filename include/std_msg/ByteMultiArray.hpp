#pragma once

#include "helper.hpp"

// R_DEFINE_ARRAY_MSG_TYPE(Byte, R_byte)

class ByteMultiArrayMsg
{
private:
    bool owns_value_memory;

public:
    const uint8_t *value;
    size_t len;
    ByteMultiArrayMsg(const uint8_t *value, size_t len) : value(value), len(len), owns_value_memory(false) {}
    ~ByteMultiArrayMsg()
    {
        if (owns_value_memory)
        {
            free((void *)value);
        }
    }
    size_t serialized_size() const { return sizeof(uint8_t) * len + sizeof(uint32_t); }
    size_t serialize(uint8_t *buffer, size_t bufSize) const
    {
        if (bufSize < sizeof(uint8_t) * len + sizeof(uint32_t))
        {
            return 0;
        }
        RosSerde serde(buffer);
        serde.writeUInt32(len);
        for (size_t i = 0; i < len; i++)
        {
            serde.writeByte(value[i]);
        }
        return serde.offset();
    }
    static ByteMultiArrayMsg *deserialize(uint8_t *buffer, size_t bufSize)
    {
        if (bufSize < sizeof(uint32_t))
        {
            return nullptr;
        }
        RosSerde serde(buffer);
        uint32_t len = serde.readUInt32();
        if (bufSize < sizeof(uint8_t) * len + sizeof(uint32_t))
        {
            return nullptr;
        }
        uint8_t *value = (uint8_t *)malloc(sizeof(uint8_t) * len);
        for (size_t i = 0; i < len; i++)
        {
            value[i] = serde.readByte();
        }
        ByteMultiArrayMsg *msg = new ByteMultiArrayMsg(value, len);
        msg->owns_value_memory = true;
        return msg;
    }
};