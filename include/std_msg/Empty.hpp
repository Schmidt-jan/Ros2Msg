#pragma once

#include <stdint.h>
#include <stddef.h>

class EmptyMsg
{
    EmptyMsg() {}

    size_t serialized_size() const
    {
        return 0;
    }

    size_t serialize() const
    {
        return 0;
    }

    static EmptyMsg *deserialize(uint8_t *buffer, size_t bufSize)
    {
        if (bufSize < 0)
        {
            return nullptr;
        }
        return new EmptyMsg();
    }
};