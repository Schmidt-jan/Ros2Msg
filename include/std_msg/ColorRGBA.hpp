#pragma once

#include "serde/ros2_serde.hpp"

class ColorRGBAMsg
{
public:
    R_Float32 r;
    R_Float32 g;
    R_Float32 b;
    R_Float32 a;

    ColorRGBAMsg(R_Float32 r, R_Float32 g, R_Float32 b, R_Float32 a) : r(r), g(g), b(b), a(a) {}


    size_t serialized_size() const
    {
        return sizeof(R_Float32) * 4;
    }   

    size_t serialize(uint8_t *buffer, size_t bufSize) const
    {
        if (bufSize < sizeof(R_Float32) * 4)
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

    static ColorRGBAMsg *deserialize(uint8_t *buffer, size_t bufSize)
    {
        if (bufSize < sizeof(R_Float32) * 4)
        {
            return nullptr;
        }
        RosSerde serde(buffer);
        R_Float32 r = serde.readFloat32();
        R_Float32 g = serde.readFloat32();
        R_Float32 b = serde.readFloat32();
        R_Float32 a = serde.readFloat32();
        return new ColorRGBAMsg(r, g, b, a);
    }
};