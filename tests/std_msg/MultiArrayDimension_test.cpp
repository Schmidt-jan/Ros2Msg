#include <gtest/gtest.h>
#include "std_msg/MultiArrayDimension.hpp"

TEST(MultiArrayDimensionTest, Serialize) {
    size_t expected_size = 16;
    uint8_t expected[expected_size] = {
        0x04, 0x00, 0x00, 0x00, // length of "test"
        't', 'e', 's', 't', // "test"
        0x0a, 0x00, 0x00, 0x00, // size
        0x14, 0x00, 0x00, 0x00, // stride
    };

    syste d("test", 10, 20);
    EXPECT_EQ(d.serialized_size(), expected_size);

    uint8_t buffer[d.serialized_size()];
    size_t size = d.serialize(buffer, d.serialized_size());
    EXPECT_EQ(size, expected_size);


    for (size_t i = 0; i < expected_size; ++i) {
        EXPECT_EQ(buffer[i], expected[i]);
    }
}

TEST(MultiArrayDimensionTest, Deserialize) {
    size_t expected_size = 16;
    uint8_t buffer[expected_size] = {
        0x04, 0x00, 0x00, 0x00, // length of "test"
        't', 'e', 's', 't', // "test"
        0x0a, 0x00, 0x00, 0x00, // size
        0x14, 0x00, 0x00, 0x00, // stride
    };

    MultiArrayDimensionMsg *d = MultiArrayDimensionMsg::deserialize(buffer, expected_size);
    EXPECT_EQ(d->label, "test");
    EXPECT_EQ(d->size, 10);
    EXPECT_EQ(d->stride, 20);

    delete d;
}