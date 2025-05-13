#pragma once
#include "serde/ros2_serde.hpp"

#define SERIALIZED_DELTA_MSG_SIZE (sizeof(int16_t) * 2)

class DeltaMsg {
public:
    int16_t delta_x;
    int16_t delta_y;

    DeltaMsg() : delta_x(0), delta_y(0) {}
    DeltaMsg(int16_t x, int16_t y) : delta_x(x), delta_y(y) {}

    size_t serialize(uint8_t* buffer, size_t maxLength) const {
        if (maxLength < SERIALIZED_DELTA_MSG_SIZE) {
            return 0;
        }
        RosSerde serde(buffer);
        serde.writeInt16(delta_x);
        serde.writeInt16(delta_y);
        return serde.offset();
    }

    static DeltaMsg *deserialize(uint8_t* buffer, size_t length) {
        if (length < SERIALIZED_DELTA_MSG_SIZE) {
            return nullptr;
        }
        RosSerde serde(buffer);
        DeltaMsg* msg = new DeltaMsg();
        msg->delta_x = serde.readInt16();
        msg->delta_y = serde.readInt16();
        return msg;
    }
};