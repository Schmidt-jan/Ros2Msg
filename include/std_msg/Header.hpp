#pragma once 

#include "builtin_interfaces/Time.hpp"

class HeaderMsg {
public:
    TimeMsg stamp;
    R_String frame_id;
    HeaderMsg(TimeMsg stamp, R_String frame_id) : stamp(stamp), frame_id(frame_id) {}

    size_t serialized_size() const {
        return stamp.serialized_size() + frame_id.size();
    }

    size_t serialize(uint8_t *buffer, size_t bufSize) const {
        if (bufSize < this->serialized_size()) {
            return 0;
        }
        RosSerde serde(buffer);
        size_t len = stamp.serialize(buffer, stamp.serialized_size());
        serde.incrementOffset(len);
        serde.writeString(frame_id);
        return serde.offset();
    }

    static HeaderMsg *deserialize(uint8_t *buffer, size_t bufSize) {
        if (bufSize < SERIALIZED_TIME_MSG_LEN) {
            return nullptr;
        }
        RosSerde serde(buffer);
        TimeMsg* stamp = TimeMsg::deserialize(buffer, SERIALIZED_TIME_MSG_LEN);
        R_String frame_id = serde.readString();
        HeaderMsg* result = new HeaderMsg(*stamp, frame_id);
        delete stamp; 
        return result;
    }
};