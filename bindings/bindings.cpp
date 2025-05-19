#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h> // <-- includes buffer support
#include <pybind11/buffer_info.h>
#include <cstring>

#include "pybind_ros2_serde.cpp"

#include "builtin_interfaces/Duration.hpp"
#include "builtin_interfaces/Time.hpp"

#include "std_msg/Bool.hpp"
#include "std_msg/Byte.hpp"
#include "std_msg/ByteMultiArray.hpp"
#include "std_msg/Char.hpp"
#include "std_msg/ColorRGBA.hpp"
#include "std_msg/Empty.hpp"
#include "std_msg/Float32.hpp"
#include "std_msg/Float32MultiArray.hpp"
#include "std_msg/Float64.hpp"
#include "std_msg/Float64MultiArray.hpp"
#include "std_msg/Header.hpp"
#include "std_msg/Int16.hpp"
#include "std_msg/Int16MultiArray.hpp"
#include "std_msg/Int32.hpp"
#include "std_msg/Int32MultiArray.hpp"
#include "std_msg/Int64.hpp"
#include "std_msg/Int64MultiArray.hpp"
#include "std_msg/Int8.hpp"
#include "std_msg/Int8MultiArray.hpp"
#include "std_msg/MultiArrayDimension.hpp"
#include "std_msg/MultiArrayLayout.hpp"
#include "std_msg/String.hpp"
#include "std_msg/UInt16.hpp"
#include "std_msg/UInt16MultiArray.hpp"
#include "std_msg/UInt32.hpp"
#include "std_msg/UInt32MultiArray.hpp"
#include "std_msg/UInt64.hpp"
#include "std_msg/UInt64MultiArray.hpp"
#include "std_msg/UInt8.hpp"
#include "std_msg/UInt8MultiArray.hpp"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

namespace py = pybind11;
// change the macro to use the init function with a parameter of type R_(lowercase type)
#define BIND_MSG_TYPE(TYPE)                             \
    py::class_<TYPE##Msg>(m, TOSTRING(TYPE##Msg))               \
        .def(py::init<R_##TYPE>())                              \
        .def("serialize", &TYPE##Msg::serialize)        \
        .def("deserialize", &TYPE##Msg::deserialize)    \
        .def("serialized_size", &TYPE##Msg::serialized_size) \
        .def_readwrite("value", &TYPE##Msg::value)

#define BIND_ARRAY_MSG_TYPE(TYPE)                             \
    py::class_<TYPE##MultiArrayMsg>(m, TOSTRING(TYPE##MultiArrayMsg))               \
        .def(py::init<const R_##TYPE *, size_t>())                              \
        .def("serialize", &TYPE##MultiArrayMsg::serialize)        \
        .def("deserialize", &TYPE##MultiArrayMsg::deserialize)    \
        .def("serialized_size", &TYPE##MultiArrayMsg::serialized_size) \
        .def_readwrite("value", &TYPE##MultiArrayMsg::value)

PYBIND11_MODULE(ros2msg, m)
{
    
    py::class_<PyRosSerde>(m, "RosSerde")
        .def(py::init<py::bytearray>())

        // Meta: bind all function names
        .def("offset", &PyRosSerde::offset)
        .def("increment_offset", &PyRosSerde::increment_offset)

        // Bind all manually (macro generated above)
        .def("write_bool", &PyRosSerde::write_bool)
        .def("read_bool", &PyRosSerde::read_bool)
        .def("write_array_bool", &PyRosSerde::write_array_bool)
        .def("read_array_bool", &PyRosSerde::read_array_bool)

        .def("write_int8", &PyRosSerde::write_int8)
        .def("read_int8", &PyRosSerde::read_int8)
        .def("write_array_int8", &PyRosSerde::write_array_int8)
        .def("read_array_int8", &PyRosSerde::read_array_int8)

        .def("write_uint8", &PyRosSerde::write_uint8)
        .def("read_uint8", &PyRosSerde::read_uint8)
        .def("write_array_uint8", &PyRosSerde::write_array_uint8)
        .def("read_array_uint8", &PyRosSerde::read_array_uint8)

        .def("write_int16", &PyRosSerde::write_int16)
        .def("read_int16", &PyRosSerde::read_int16)
        .def("write_array_int16", &PyRosSerde::write_array_int16)
        .def("read_array_int16", &PyRosSerde::read_array_int16)

        .def("write_uint16", &PyRosSerde::write_uint16)
        .def("read_uint16", &PyRosSerde::read_uint16)
        .def("write_array_uint16", &PyRosSerde::write_array_uint16)
        .def("read_array_uint16", &PyRosSerde::read_array_uint16)

        .def("write_int32", &PyRosSerde::write_int32)
        .def("read_int32", &PyRosSerde::read_int32)
        .def("write_array_int32", &PyRosSerde::write_array_int32)
        .def("read_array_int32", &PyRosSerde::read_array_int32)

        .def("write_uint32", &PyRosSerde::write_uint32)
        .def("read_uint32", &PyRosSerde::read_uint32)
        .def("write_array_uint32", &PyRosSerde::write_array_uint32)
        .def("read_array_uint32", &PyRosSerde::read_array_uint32)

        .def("write_int64", &PyRosSerde::write_int64)
        .def("read_int64", &PyRosSerde::read_int64)
        .def("write_array_int64", &PyRosSerde::write_array_int64)
        .def("read_array_int64", &PyRosSerde::read_array_int64)

        .def("write_uint64", &PyRosSerde::write_uint64)
        .def("read_uint64", &PyRosSerde::read_uint64)
        .def("write_array_uint64", &PyRosSerde::write_array_uint64)
        .def("read_array_uint64", &PyRosSerde::read_array_uint64)

        .def("write_float32", &PyRosSerde::write_float32)
        .def("read_float32", &PyRosSerde::read_float32)
        .def("write_array_float32", &PyRosSerde::write_array_float32)
        .def("read_array_float32", &PyRosSerde::read_array_float32)

        .def("write_float64", &PyRosSerde::write_float64)
        .def("read_float64", &PyRosSerde::read_float64)
        .def("write_array_float64", &PyRosSerde::write_array_float64)
        .def("read_array_float64", &PyRosSerde::read_array_float64)

        .def("write_string", &PyRosSerde::write_string)
        .def("read_string", &PyRosSerde::read_string)
        .def("write_array_string", &PyRosSerde::write_array_string)
        .def("read_array_string", &PyRosSerde::read_array_string);


    
    // Bind all message types
    BIND_MSG_TYPE(Bool);
    BIND_MSG_TYPE(Byte);
    BIND_MSG_TYPE(Char);
    BIND_MSG_TYPE(Int8);
    BIND_MSG_TYPE(Int16);
    BIND_MSG_TYPE(Int32);
    BIND_MSG_TYPE(Int64);
    BIND_MSG_TYPE(UInt8);
    BIND_MSG_TYPE(UInt16);
    BIND_MSG_TYPE(UInt32);
    BIND_MSG_TYPE(UInt64);
    BIND_MSG_TYPE(Float32);
    BIND_MSG_TYPE(Float64);
    BIND_MSG_TYPE(String);

    BIND_ARRAY_MSG_TYPE(Byte);
    BIND_ARRAY_MSG_TYPE(Int8);
    BIND_ARRAY_MSG_TYPE(Int16);
    BIND_ARRAY_MSG_TYPE(Int32);
    BIND_ARRAY_MSG_TYPE(Int64);
    BIND_ARRAY_MSG_TYPE(UInt8);
    BIND_ARRAY_MSG_TYPE(UInt16);
    BIND_ARRAY_MSG_TYPE(UInt32);
    BIND_ARRAY_MSG_TYPE(UInt64);
    BIND_ARRAY_MSG_TYPE(Float32);
    BIND_ARRAY_MSG_TYPE(Float64);
    

    py::class_<ColorRGBAMsg>(m, "ColorRGBAMsg")
        .def(py::init<R_Float32, R_Float32, R_Float32, R_Float32>())
        .def("serialize", &ColorRGBAMsg::serialize)
        .def("deserialize", &ColorRGBAMsg::deserialize)
        .def("serialized_size", &ColorRGBAMsg::serialized_size)
        .def_readwrite("r", &ColorRGBAMsg::r)
        .def_readwrite("g", &ColorRGBAMsg::g)
        .def_readwrite("b", &ColorRGBAMsg::b)
        .def_readwrite("a", &ColorRGBAMsg::a);

    py::class_<HeaderMsg>(m, "HeaderMsg")
        .def(py::init<TimeMsg, R_String>())
        .def("serialize", &HeaderMsg::serialize)
        .def("deserialize", &HeaderMsg::deserialize)
        .def("serialized_size", &HeaderMsg::serialized_size)
        .def_readwrite("stamp", &HeaderMsg::stamp)
        .def_readwrite("frame_id", &HeaderMsg::frame_id);

    py::class_<EmptyMsg>(m, "EmptyMsg")
        .def(py::init<>())
        .def("serialize", &EmptyMsg::serialize)
        .def("deserialize", &EmptyMsg::deserialize)
        .def("serialized_size", &EmptyMsg::serialized_size);

    py::class_<MultiArrayDimensionMsg>(m, "MultiArrayDimensionMsg")
        .def(py::init<R_String, R_UInt32, R_UInt32>())
        .def("serialize", &MultiArrayDimensionMsg::serialize)
        .def("deserialize", &MultiArrayDimensionMsg::deserialize)
        .def("serialized_size", &MultiArrayDimensionMsg::serialized_size)
        .def_readwrite("label", &MultiArrayDimensionMsg::label)
        .def_readwrite("size", &MultiArrayDimensionMsg::size)
        .def_readwrite("stride", &MultiArrayDimensionMsg::stride);

    py::class_<MultiArrayLayout>(m, "MultiArrayLayout")
        .def(py::init<MultiArrayDimensionMsg *, uint32_t, R_UInt32>())
        .def("serialize", &MultiArrayLayout::serialize)
        .def("deserialize", &MultiArrayLayout::deserialize)
        .def("serialized_size", &MultiArrayLayout::serialized_size)
        .def_readwrite("dim", &MultiArrayLayout::dim)
        .def_readwrite("dim_size", &MultiArrayLayout::dim_size)
        .def_readwrite("data_offset", &MultiArrayLayout::data_offset);


    // builtin_interfaces
    py::class_<DurationMsg>(m, "DurationMsg")
        .def(py::init<uint32_t, uint32_t>())
        .def("serialize", &DurationMsg::serialize)
        .def("deserialize", &DurationMsg::deserialize)
        .def("serialized_size", &DurationMsg::serialized_size)
        .def_readwrite("sec", &DurationMsg::sec)
        .def_readwrite("nanosec", &DurationMsg::nanosec);
*
    py::class_<TimeMsg>(m, "TimeMsg")
        .def(py::init<uint32_t, uint32_t>())
        .def("serialize", &TimeMsg::serialize)
        .def("deserialize", &TimeMsg::deserialize)
        .def("serialized_size", &TimeMsg::serialized_size)
        .def_readwrite("sec", &TimeMsg::sec)
        .def_readwrite("nanosec", &TimeMsg::nanosec);
        
}