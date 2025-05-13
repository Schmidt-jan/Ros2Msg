#pragma once
#include "serde/ros2_serde.hpp"
#include "MultiArrayDimension.hpp"

class MultiArrayLayout
{
private:
    bool owns_dim_memory; // Indicates if the class owns the memory for dim

public:
    uint32_t dim_size;
    MultiArrayDimensionMsg *dim;
    R_uint32 data_offset;


    MultiArrayLayout(MultiArrayDimensionMsg *dim, uint32_t dim_size, R_uint32 data_offset) : dim(dim), dim_size(dim_size), data_offset(data_offset), owns_dim_memory(false) {}

    ~MultiArrayLayout()
    {
        if (owns_dim_memory)
        {
            delete[] dim; // Free the allocated memory for dim
        }
    }

    size_t serialized_size() const
    {
        size_t size = sizeof(uint32_t); // dim_size
        for (uint32_t i = 0; i < dim_size; i++)
        {
            size += dim[i].serialized_size();
        }
        size += sizeof(R_uint32); // data_offset
        return size;
    }

    size_t serialize(uint8_t *buffer, size_t bufSize) const
    {
        RosSerde serde(buffer);
        serde.writeUInt32(dim_size);
        for (uint32_t i = 0; i < dim_size; i++)
        {
            size_t ser_size = dim[i].serialized_size();
            uint8_t dim_buffer[ser_size];
            dim[i].serialize(dim_buffer, ser_size);
            serde.write(dim_buffer, ser_size);
        }
        serde.writeUInt32(data_offset);
        return serde.offset();
    }

    static MultiArrayLayout *deserialize(uint8_t *buffer, size_t bufSize)
    {
        RosSerde serde(buffer);
        if (bufSize < sizeof(uint32_t))
            return nullptr;

        uint32_t dim_size = serde.readUInt32();

        // Allocate memory for the dimensions array
        MultiArrayDimensionMsg *dim_array = new MultiArrayDimensionMsg[dim_size];
        if (!dim_array)
            return nullptr; // Memory allocation failed

        for (uint32_t i = 0; i < dim_size; i++)
        {
            if (serde.offset() >= bufSize)
            {
                // Cleanup and return nullptr if deserialization is not possible
                delete[] dim_array;
                return nullptr;
            }

            uint8_t *start = buffer + serde.offset();
            size_t remaining_size = bufSize - serde.offset();
            auto val = MultiArrayDimensionMsg::deserialize(start, remaining_size);

            if (!val)
            {
                // Cleanup and return nullptr if nested deserialization failed
                delete[] dim_array;
                return nullptr;
            }

            auto ser_size = val->serialized_size();
            if (serde.offset() + ser_size > bufSize)
            {
                // Cleanup and return nullptr if serialized size exceeds buffer
                delete val;
                delete[] dim_array;
                return nullptr;
            }

            dim_array[i] = *val;
            delete val; // Free the memory allocated in deserialize
            serde.incrementOffset(ser_size);
        }

        if (serde.offset() + sizeof(R_uint32) > bufSize)
        {
            // Cleanup and return nullptr if data_offset cannot be read
            delete[] dim_array;
            return nullptr;
        }

        R_uint32 data_offset = serde.readUInt32();
        auto result = new MultiArrayLayout(dim_array, dim_size, data_offset);
        result->owns_dim_memory = true; // Set owns_dim_memory to true to indicate ownership of dim_array
        return result;
    }
};
