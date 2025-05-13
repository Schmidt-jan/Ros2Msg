#pragma once

#include "serde/ros2_serde.hpp"

class ColorRGBA
{
public:
    R_float32 r;
    R_float32 g;
    R_float32 b;
    R_float32 a;

    ColorRGBA(R_float32 r, R_float32 g, R_float32 b, R_float32 a) : r(r), g(g), b(b), a(a) {}


    size_t serialized_size() const
    {
        return sizeof(R_float32) * 4;
    }   

    size_t serialize(uint8_t *buffer, size_t bufSize) const
    {
        if (bufSize < sizeof(R_float32) * 4)
        {
            return 0;
        }
        RosSerde serde(buffer);
        serde.writeFloat32(r);
        serde.writeFloat32(g);
        serde.writeFloat32(b);
        serde.writeFloat32(a);
        return serde.offset();
    }

    static ColorRGBA *deserialize(uint8_t *buffer, size_t bufSize)
    {
        if (bufSize < sizeof(R_float32) * 4)
        {
            return nullptr;
        }
        RosSerde serde(buffer);
        R_float32 r = serde.readFloat32();
        R_float32 g = serde.readFloat32();
        R_float32 b = serde.readFloat32();
        R_float32 a = serde.readFloat32();
        return new ColorRGBA(r, g, b, a);
    }
};