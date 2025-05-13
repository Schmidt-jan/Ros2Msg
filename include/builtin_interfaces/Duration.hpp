#pragma once

#include "serde/ros2_serde.hpp"

class DurationMsg
{
public:
    uint32_t sec;
    uint32_t nanosec;

    DurationMsg(uint32_t sec, uint32_t nanosec) : sec(sec), nanosec(nanosec) {}

    size_t serialize(uint8_t *buffer, size_t bufSize) const
    {
        if (bufSize < sizeof(uint32_t) * 2)
        {
            return 0;
        }
        RosSerde serde(buffer);
        serde.writeUInt32(sec);
        serde.writeUInt32(nanosec);
        return serde.offset();
    }

    static DurationMsg *deserialize(uint8_t *buffer, size_t bufSize)
    {
        if (bufSize < sizeof(uint32_t) * 2)
        {
            return 0;
        }
        RosSerde serde(buffer);
        R_uint32 sec = serde.readUInt32();
        R_uint32 nanosec = serde.readUInt32();
        return new DurationMsg(sec, nanosec);
    }
};