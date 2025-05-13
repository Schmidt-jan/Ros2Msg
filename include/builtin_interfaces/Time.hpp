#pragma once

#include "serde/ros2_serde.hpp"

#define SERIALIZED_TIME_MSG_LEN (sizeof(uint32_t) * 2)

class TimeMsg
{
public:
    uint32_t sec;
    uint32_t nanosec;

    TimeMsg(uint32_t sec, uint32_t nanosec) : sec(sec), nanosec(nanosec) {}

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

    static TimeMsg *deserialize(uint8_t *buffer, size_t bufSize)
    {
        if (bufSize < sizeof(uint32_t) * 2)
        {
            return 0;
        }
        RosSerde serde(buffer);
        R_uint32 sec = serde.readUInt32();
        R_uint32 nanosec = serde.readUInt32();
        return new TimeMsg(sec, nanosec);
    }
};