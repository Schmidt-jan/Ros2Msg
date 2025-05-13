#pragma once

#include "helper.hpp"

R_DEFINE_MSG_TYPE(Int8, R_int8)
class Int8Msg { public: int8_t value; Int8Msg(int8_t value) : value(value) {} size_t serialized_size() const { return sizeof(int8_t); } size_t serialize(uint8_t *buffer, size_t bufSize) const { if (bufSize < sizeof(int8_t)) { return 0; } RosSerde serde(buffer); serde.writeInt8(value); return serde.offset(); } static Int8Msg* deserialize(uint8_t *buffer, size_t bufSize) { if (bufSize < sizeof(int8_t)) { return 0; } RosSerde serde(buffer); int8_t value = serde.readInt8(); return new Int8Msg(value); } };