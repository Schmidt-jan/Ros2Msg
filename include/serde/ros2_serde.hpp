#pragma once
#include "ros2_types.h"

class RosSerde
{
public:
    RosSerde(uint8_t *buffer) : buffer_(buffer), offset_(0) {}

    // Write methods
    void writeBool(R_Bool value)            { write<R_Bool>(value ? 1 : 0); }
    void writeByte(R_Byte value)            { write<R_Byte>(value); }
    void writeChar(R_Char value)            { write<R_Char>(value); }
    void writeFloat32(R_Float32 value)      { write<R_Float32>(value); }
    void writeFloat64(R_Float64 value)      { write<R_Float64>(value); }
    void writeInt8(R_Int8 value)            { write<R_Int8>(value); }
    void writeUInt8(R_UInt8 value)          { write<R_UInt8>(value); }
    void writeInt16(R_Int16 value)          { write<R_Int16>(value); }
    void writeUInt16(R_UInt16 value)        { write<R_UInt16>(value); }
    void writeInt32(R_Int32 value)          { write<R_Int32>(value); }
    void writeUInt32(R_UInt32 value)        { write<R_UInt32>(value); }
    void writeInt64(R_Int64 value)          { write<R_Int64>(value); }
    void writeUInt64(R_UInt64 value)        { write<R_UInt64>(value); }

    void writeString(const R_String str)
    {
        write<uint32_t>(static_cast<uint32_t>(str.size()));
        memcpy(buffer_ + offset_, str.data(), str.size());
        offset_ += str.size();
    }

    void writeWString(const R_Wstring &wstr)
    {
        uint32_t len = static_cast<uint32_t>(wstr.size());
        write<uint32_t>(len);
        memcpy(buffer_ + offset_, wstr.data(), len * sizeof(char16_t));
        offset_ += len * sizeof(char16_t);
    }

    void write(const uint8_t *data, size_t size)
    {
        memcpy(buffer_ + offset_, data, size);
        offset_ += size;
    }

    // === Generic Array Write ===
    template <typename T>
    void writeArray(const T *array, uint32_t count)
    {
        write<uint32_t>(count);
        for (uint32_t i = 0; i < count; ++i)
            write<T>(array[i]);
    }

    // === String Array Specialization ===
    void writeArray(const R_String *array, uint32_t count)
    {
        write<uint32_t>(count);
        for (uint32_t i = 0; i < count; ++i)
            writeString(array[i]);
    }

    // === WString Array Specialization ===
    void writeArray(const R_Wstring *array, uint32_t count)
    {
        write<uint32_t>(count);
        for (uint32_t i = 0; i < count; ++i)
            writeWString(array[i]);
    }

    // Read methods
    R_Bool readBool()       { return read<R_Bool>() != 0; }
    R_Byte readByte()       { return read<R_Byte>(); }
    R_Char readChar()       { return read<R_Char>(); }
    R_Float32 readFloat32() { return read<R_Float32>(); }
    R_Float64 readFloat64() { return read<R_Float64>(); }
    R_Int8 readInt8()       { return read<R_Int8>(); }
    R_UInt8 readUInt8()     { return read<R_UInt8>(); }
    R_Int16 readInt16()     { return read<R_Int16>(); }
    R_UInt16 readUInt16()   { return read<R_UInt16>(); }
    R_Int32 readInt32()     { return read<R_Int32>(); }
    R_UInt32 readUInt32()   { return read<R_UInt32>(); }
    R_Int64 readInt64()     { return read<R_Int64>(); }
    R_UInt64 readUInt64()   { return read<R_UInt64>(); }

    R_String readString()
    {
        uint32_t len = read<uint32_t>();
        R_String str(reinterpret_cast<char *>(buffer_ + offset_), len);
        offset_ += len;
        return str;
    }

    R_Wstring readWString()
    {
        uint32_t len = read<uint32_t>();
        R_Wstring wstr(reinterpret_cast<char16_t *>(buffer_ + offset_), len);
        offset_ += len * sizeof(char16_t);
        return wstr;
    }

    void read(uint8_t *data, size_t size)
    {
        memcpy(data, buffer_ + offset_, size);
        offset_ += size;
    }

    // === Generic Array Read ===
    template <typename T>
    void readArray(T *arrayOut, uint32_t &countOut)
    {
        countOut = read<uint32_t>();
        for (uint32_t i = 0; i < countOut; ++i)
            arrayOut[i] = read<T>();
    }

    // === String Array Read ===
    void readArray(R_String *arrayOut, uint32_t &countOut)
    {
        countOut = read<uint32_t>();
        for (uint32_t i = 0; i < countOut; ++i)
            arrayOut[i] = readString();
    }

    // === WString Array Read ===
    void readArray(R_Wstring *arrayOut, uint32_t &countOut)
    {
        countOut = read<uint32_t>();
        for (uint32_t i = 0; i < countOut; ++i)
            arrayOut[i] = readWString();
    }

    size_t offset() const { return offset_; }
    void incrementOffset(size_t value) { offset_ += value; }

private:
    template <typename T>
    void write(const T &value)
    {
        memcpy(buffer_ + offset_, &value, sizeof(T));
        offset_ += sizeof(T);
    }

    template <typename T>
    T read()
    {
        T value;
        memcpy(&value, buffer_ + offset_, sizeof(T));
        offset_ += sizeof(T);
        return value;
    }

    uint8_t *buffer_;
    size_t offset_;
};
