#include <gtest/gtest.h>
#include "std_msg/ByteMultiArray.hpp"

TEST(ByteMultiArrayTest, Serialize) {
    size_t expected_size = 6;
    uint8_t expected[] = {
        0x02, 0x00, 0x00, 0x00, // dim_size
        0x12,
        0x42,
    };

    const uint8_t data[] = {0x12, 0x42};
    ByteMultiArrayMsg msg(data, sizeof(data) / sizeof(data[0]));
    
    EXPECT_EQ(msg.serialized_size(), expected_size);
    uint8_t buffer[msg.serialized_size()];
    size_t size = msg.serialize(buffer, msg.serialized_size());
    EXPECT_EQ(size, expected_size);
    for (size_t i = 0; i < expected_size; ++i) {
        EXPECT_EQ(buffer[i], expected[i]);
    }
}

TEST(ByteMultiArrayTest, Deserialize) {
    size_t expected_size = 6;
    uint8_t buffer[expected_size] = {
        0x02, 0x00, 0x00, 0x00, // dim_size
        0x12,
        0x42,
    };

    ByteMultiArrayMsg *msg = ByteMultiArrayMsg::deserialize(buffer, expected_size);
    EXPECT_EQ(msg->len, (R_UInt32)2);
    EXPECT_EQ(msg->value[0], (R_Byte)0x12);
    EXPECT_EQ(msg->value[1], (R_Byte)0x42);

    delete msg;
}