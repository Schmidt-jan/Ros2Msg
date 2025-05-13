#pragma once

#include "serde/ros2_serde.hpp"
#include "serde/ros2_types.h"

#define R_DEFINE_MSG_TYPE(name, type)                                  \
    class name##Msg                                                    \
    {                                                                  \
    public:                                                            \
        type value;                                                    \
        name##Msg(type value) : value(value) {}                        \
        size_t serialized_size() const                                 \
        {                                                              \
            return sizeof(type);                                       \
        }                                                              \
        size_t serialize(uint8_t *buffer, size_t bufSize) const        \
        {                                                              \
            if (bufSize < sizeof(type))                                \
            {                                                          \
                return 0;                                              \
            }                                                          \
            RosSerde serde(buffer);                                    \
            serde.write##name(value);                                  \
            return serde.offset();                                     \
        }                                                              \
        static name##Msg *deserialize(uint8_t *buffer, size_t bufSize) \
        {                                                              \
            if (bufSize < sizeof(type))                                \
            {                                                          \
                return 0;                                              \
            }                                                          \
            RosSerde serde(buffer);                                    \
            type value = serde.read##name();                           \
            return new name##Msg(value);                               \
        }                                                              \
    };

#define R_DEFINE_ARRAY_MSG_TYPE(name, type)                                                                      \
    class name##MultiArrayMsg                                                                                    \
    {                                                                                                            \
    private:                                                                                                    \
        bool owns_value_memory;                                                                                  \
    public:                                                                                                      \
        const type *value;                                                                                       \
        size_t len;                                                                                              \
        name##MultiArrayMsg(const type *value, size_t len) : value(value), len(len), owns_value_memory(false) {} \
        ~name##MultiArrayMsg()                                                                                   \
        {                                                                                                        \
            if (owns_value_memory)                                                                               \
            {                                                                                                    \
                free((void *)value);                                                                             \
            }                                                                                                    \
        }                                                                                                        \
        size_t serialized_size() const                                                                           \
        {                                                                                                        \
            return sizeof(type) * len + sizeof(uint32_t);                                                        \
        }                                                                                                        \
        size_t serialize(uint8_t *buffer, size_t bufSize) const                                                  \
        {                                                                                                        \
            if (bufSize < sizeof(type) * len + sizeof(uint32_t))                                                 \
            {                                                                                                    \
                return 0;                                                                                        \
            }                                                                                                    \
            RosSerde serde(buffer);                                                                              \
            serde.writeUInt32(len);                                                                              \
            for (size_t i = 0; i < len; i++)                                                                     \
            {                                                                                                    \
                serde.write##name(value[i]);                                                                     \
            }                                                                                                    \
            return serde.offset();                                                                               \
        }                                                                                                        \
        static name##MultiArrayMsg *deserialize(uint8_t *buffer, size_t bufSize)                                 \
        {                                                                                                        \
            if (bufSize < sizeof(uint32_t))                                                                      \
            {                                                                                                    \
                return nullptr;                                                                                  \
            }                                                                                                    \
            RosSerde serde(buffer);                                                                              \
            uint32_t len = serde.readUInt32();                                                                   \
            if (bufSize < sizeof(type) * len + sizeof(uint32_t))                                                 \
            {                                                                                                    \
                return nullptr;                                                                                  \
            }                                                                                                    \
            type *value = (type *)malloc(sizeof(type) * len);                                                    \
            for (size_t i = 0; i < len; i++)                                                                     \
            {                                                                                                    \
                value[i] = serde.read##name();                                                                   \
            }                                                                                                    \
            name##MultiArrayMsg *msg = new name##MultiArrayMsg(value, len);                                      \
            msg->owns_value_memory = true;                                                                       \
            return msg;                                                                                          \
        }                                                                                                        \
    };