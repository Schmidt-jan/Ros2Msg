#pragma once
#include "serde/ros2_serde.hpp"

#define SERIALIZED_DELTA_MSG_SIZE (sizeof(int16_t) * 2 + sizeof(bool) * 2)

class DeltaMsg {
public:
    int16_t delta_x;
    int16_t delta_y;
    bool delta_x_ovfl;
    bool delta_y_ovfl;

    DeltaMsg() : delta_x(0), delta_y(0) {}
    DeltaMsg(int16_t x, int16_t y) : delta_x(x), delta_y(y), delta_x_ovfl(false), delta_y_ovfl(false) {}
    DeltaMsg(int16_t x, int16_t y, bool x_ovfl, bool y_ovfl) : delta_x(x), delta_y(y), delta_x_ovfl(x_ovfl), delta_y_ovfl(y_ovfl) {}

    static size_t serialized_size() {
        return SERIALIZED_DELTA_MSG_SIZE;
    }

    size_t serialize(uint8_t* buffer, size_t maxLength) const {
        if (maxLength < SERIALIZED_DELTA_MSG_SIZE) {
            return 0;
        }
        RosSerde serde(buffer);
        serde.writeInt16(delta_x);
        serde.writeInt16(delta_y);
        serde.writeBool(delta_x_ovfl);
        serde.writeBool(delta_y_ovfl);
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
        msg->delta_x_ovfl = serde.readBool();
        msg->delta_y_ovfl = serde.readBool();
        return msg;
    }
};