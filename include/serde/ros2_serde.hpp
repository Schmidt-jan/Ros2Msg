#pragma once
#include "ros2_types.h"

class RosSerde
{
public:
    RosSerde(uint8_t *buffer) : buffer_(buffer), offset_(0) {}

    // Write methods
    void writeBool(R_bool value)            { write<R_bool>(value ? 1 : 0); }
    void writeByte(R_byte value)            { write<R_byte>(value); }
    void writeChar(R_char value)            { write<R_char>(value); }
    void writeFloat32(R_float32 value)      { write<R_float32>(value); }
    void writeFloat64(R_float64 value)      { write<R_float64>(value); }
    void writeInt8(R_int8 value)            { write<R_int8>(value); }
    void writeUInt8(R_uint8 value)          { write<R_uint8>(value); }
    void writeInt16(R_int16 value)          { write<R_int16>(value); }
    void writeUInt16(R_uint16 value)        { write<R_uint16>(value); }
    void writeInt32(R_int32 value)          { write<R_int32>(value); }
    void writeUInt32(R_uint32 value)        { write<R_uint32>(value); }
    void writeInt64(R_int64 value)          { write<R_int64>(value); }
    void writeUInt64(R_uint64 value)        { write<R_uint64>(value); }

    void writeString(const R_string str)
    {
        write<uint32_t>(static_cast<uint32_t>(str.size()));
        memcpy(buffer_ + offset_, str.data(), str.size());
        offset_ += str.size();
    }

    void writeWString(const R_wstring &wstr)
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
    void writeArray(const R_string *array, uint32_t count)
    {
        write<uint32_t>(count);
        for (uint32_t i = 0; i < count; ++i)
            writeString(array[i]);
    }

    // === WString Array Specialization ===
    void writeArray(const R_wstring *array, uint32_t count)
    {
        write<uint32_t>(count);
        for (uint32_t i = 0; i < count; ++i)
            writeWString(array[i]);
    }

    // Read methods
    R_bool readBool()       { return read<R_bool>() != 0; }
    R_byte readByte()       { return read<R_byte>(); }
    R_char readChar()       { return read<R_char>(); }
    R_float32 readFloat32() { return read<R_float32>(); }
    R_float64 readFloat64() { return read<R_float64>(); }
    R_int8 readInt8()       { return read<R_int8>(); }
    R_uint8 readUInt8()     { return read<R_uint8>(); }
    R_int16 readInt16()     { return read<R_int16>(); }
    R_uint16 readUInt16()   { return read<R_uint16>(); }
    R_int32 readInt32()     { return read<R_int32>(); }
    R_uint32 readUInt32()   { return read<R_uint32>(); }
    R_int64 readInt64()     { return read<R_int64>(); }
    R_uint64 readUInt64()   { return read<R_uint64>(); }

    R_string readString()
    {
        uint32_t len = read<uint32_t>();
        R_string str(reinterpret_cast<char *>(buffer_ + offset_), len);
        offset_ += len;
        return str;
    }

    R_wstring readWString()
    {
        uint32_t len = read<uint32_t>();
        R_wstring wstr(reinterpret_cast<char16_t *>(buffer_ + offset_), len);
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
    void readArray(R_string *arrayOut, uint32_t &countOut)
    {
        countOut = read<uint32_t>();
        for (uint32_t i = 0; i < countOut; ++i)
            arrayOut[i] = readString();
    }

    // === WString Array Read ===
    void readArray(R_wstring *arrayOut, uint32_t &countOut)
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
