#ifdef ROS2MSG_PYTHON_BUILDING_ENABLED

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>        // <-- includes buffer support
#include <pybind11/buffer_info.h>
#include <cstring>

#include "serde/ros2_serde.hpp"

namespace py = pybind11;

class PyRosSerde {
public:
    explicit PyRosSerde(py::buffer buffer) {
        py::buffer_info info = buffer.request();

        if (info.ndim != 1 || info.itemsize != 1)
            throw std::runtime_error("Expected a 1D byte buffer");

        buffer_data_ = static_cast<uint8_t *>(info.ptr);
        buffer_size_ = info.size;
        serde_ = std::make_unique<RosSerde>(buffer_data_);
    }

    size_t offset() const { return serde_->offset(); }
    void increment_offset(size_t value) { serde_->incrementOffset(value); }

    // === Macros ===
#define DEFINE_RW(py_name, cpp_type, write_fn, read_fn) \
    void write_##py_name(cpp_type val) { serde_->write_fn(val); } \
    cpp_type read_##py_name() { return serde_->read_fn(); } \
    void write_array_##py_name(const std::vector<cpp_type>& vals) { \
        serde_->writeArray(vals.data(), static_cast<uint32_t>(vals.size())); \
    } \
    std::vector<cpp_type> read_array_##py_name() { \
        uint32_t count = 0; \
        std::vector<cpp_type> result(1024); \
        serde_->readArray(result.data(), count); \
        result.resize(count); \
        return result; \
    }

    // Supported Types
    DEFINE_RW(bool,    R_Bool,    writeBool,    readBool)
    DEFINE_RW(int8,    R_Int8,    writeInt8,    readInt8)
    DEFINE_RW(uint8,   R_UInt8,   writeUInt8,   readUInt8)
    DEFINE_RW(int16,   R_Int16,   writeInt16,   readInt16)
    DEFINE_RW(uint16,  R_UInt16,  writeUInt16,  readUInt16)
    DEFINE_RW(int32,   R_Int32,   writeInt32,   readInt32)
    DEFINE_RW(uint32,  R_UInt32,  writeUInt32,  readUInt32)
    DEFINE_RW(int64,   R_Int64,   writeInt64,   readInt64)
    DEFINE_RW(uint64,  R_UInt64,  writeUInt64,  readUInt64)
    DEFINE_RW(float32, R_Float32, writeFloat32, readFloat32)
    DEFINE_RW(float64, R_Float64, writeFloat64, readFloat64)

#undef DEFINE_RW

    // === Special case: string ===
    void write_string(const R_String &val) { serde_->writeString(val); }
    R_String read_string() { return serde_->readString(); }

    void write_array_string(const std::vector<R_String>& vals) {
        serde_->writeArray(vals.data(), static_cast<uint32_t>(vals.size()));
    }

    std::vector<R_String> read_array_string() {
        uint32_t count = 0;
        std::vector<R_String> out(1024);
        serde_->readArray(out.data(), count);
        out.resize(count);
        return out;
    }

private:
    uint8_t *buffer_data_;
    size_t buffer_size_;
    std::unique_ptr<RosSerde> serde_;
};


#endif