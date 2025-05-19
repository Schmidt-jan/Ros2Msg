#pragma once
#include "serde/ros2_serde.hpp"
#include "Delta.hpp"

class DeltaMultiArrayMsg
{
private:
    bool owns_dim_memory;

public:
    DeltaMsg *data;
    int32_t size;
    DeltaMultiArrayMsg() : data(nullptr), size(0) {}

    DeltaMultiArrayMsg(DeltaMsg *data, int32_t size) : data(data), size(size), owns_dim_memory(false) {}

    ~DeltaMultiArrayMsg()
    {
        if (owns_dim_memory)
        {
            delete[] data; 
        }
    }

    size_t serialized_size() const
    {
        return sizeof(int32_t) + size * DeltaMsg::serialized_size();
    }

    size_t serialize(uint8_t *buffer, size_t maxLength) const
    {
        if (maxLength < serialized_size())
        {
            return 0;
        }
        RosSerde serde(buffer);
        serde.writeInt32(size);
        for (int i = 0; i < size; i++)
        {
            size_t ser_size = data[i].serialized_size();
            uint8_t dim_buffer[ser_size];
            data[i].serialize(dim_buffer, ser_size);
            serde.write(dim_buffer, ser_size);
        }
        return serialized_size();
    }
    static DeltaMultiArrayMsg *deserialize(uint8_t *buffer, size_t bufSize)
    {
        if (bufSize < sizeof(uint32_t))
        return nullptr;
    
    RosSerde serde(buffer);
    uint32_t dim_size = serde.readUInt32();

    // Allocate memory for the dimensions array
    DeltaMultiArrayMsg *delta_array = new DeltaMultiArrayMsg[dim_size];
    if (!delta_array)
        return nullptr; // Memory allocation failed

    for (uint32_t i = 0; i < dim_size; i++)
    {
        if (serde.offset() >= bufSize)
        {
            // Cleanup and return nullptr if deserialization is not possible
            delete[] delta_array;
            return nullptr;
        }

        uint8_t *start = buffer + serde.offset();
        size_t remaining_size = bufSize - serde.offset();
        auto val = DeltaMultiArrayMsg::deserialize(start, remaining_size);

        if (!val)
        {
            // Cleanup and return nullptr if nested deserialization failed
            delete[] delta_array;
            return nullptr;
        }

        auto ser_size = val->serialized_size();
        if (serde.offset() + ser_size > bufSize)
        {
            // Cleanup and return nullptr if serialized size exceeds buffer
            delete val;
            delete[] delta_array;
            return nullptr;
        }

        delta_array[i] = *val;
        delete val; // Free the memory allocated in deserialize
        serde.incrementOffset(ser_size);
    }

    if (serde.offset() + sizeof(R_UInt32) > bufSize)
    {
        // Cleanup and return nullptr if data_offset cannot be read
        delete[] delta_array;
        return nullptr;
    }

    R_UInt32 data_offset = serde.readUInt32();
    auto result = new DeltaMultiArrayMsg(delta_array, dim_size, data_offset);
    result->owns_dim_memory = true; // Set owns_dim_memory to true to indicate ownership of delta_array
    return result;
    }
};