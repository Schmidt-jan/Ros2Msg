#pragma once
#include "serde/ros2_serde.hpp"

class MultiArrayDimensionMsg
{
public:
    R_string label;
    R_uint32 size;
    R_uint32 stride;

    MultiArrayDimensionMsg() : label(""), size(0), stride(0) {}
    MultiArrayDimensionMsg(R_string label, R_uint32 size, R_uint32 stride) : label(label), size(size), stride(stride) {}
    
    size_t serialized_size() const
    {
        return sizeof(uint32_t) + label.size() + sizeof(R_uint32) * 2;
    }

    size_t serialize(uint8_t *buffer, size_t bufSize) const
    {
        if (bufSize < this->serialized_size())
        {
            return 0;
        }
        RosSerde serde(buffer);
        serde.writeString(label);
        serde.writeUInt32(size);
        serde.writeUInt32(stride);
        return serde.offset();
    }
    
    static MultiArrayDimensionMsg *deserialize(uint8_t *buffer, size_t bufSize)
    {
        if (bufSize < sizeof(R_uint32) * 2)
        {
            return nullptr;
        }
        RosSerde serde(buffer);
        R_string label = serde.readString();
        R_uint32 size = serde.readUInt32();
        R_uint32 stride = serde.readUInt32();
        return new MultiArrayDimensionMsg(label, size, stride);
    }
};