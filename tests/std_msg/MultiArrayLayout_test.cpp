#include <gtest/gtest.h>
#include "std_msg/MultiArrayLayout.hpp"

TEST(MultiArrayLayoutTest, Serialize) {
    size_t expected_size = 41;
    uint8_t expected[expected_size] = {
        0x02, 0x00, 0x00, 0x00, // dim_size

        0x04, 0x00, 0x00, 0x00, // length of "test"
        't', 'e', 's', 't', // "test"
        0x0a, 0x00, 0x00, 0x00, // size
        0x14, 0x00, 0x00, 0x00, // stride

        0x05, 0x00, 0x00, 0x00, // length of "test2"
        't', 'e', 's', 't', '2', // "test2"
        0x14, 0x00, 0x00, 0x00, // size
        0x28, 0x00, 0x00, 0x00, // stride

        0x1c, 0x00, 0x00, 0x00 // data_offset
    };

    MultiArrayDimensionMsg dim1("test", 10, 20);
    MultiArrayDimensionMsg dim2("test2", 20, 40);
    MultiArrayDimensionMsg dims[] = {dim1, dim2};
    MultiArrayLayout layout(dims, sizeof(dims) / sizeof(dims[0]), (R_UInt32)28);

    EXPECT_EQ(layout.serialized_size(), expected_size);

    uint8_t buffer[layout.serialized_size()];
    size_t size = layout.serialize(buffer, layout.serialized_size());
    EXPECT_EQ(size, expected_size);


    for (size_t i = 0; i < expected_size; ++i) {
        EXPECT_EQ(buffer[i], expected[i]);
    }
}
TEST(MultiArrayLayoutTest, Deserialize) {
    size_t expected_size = 41;
    uint8_t buffer[expected_size] = {
        0x02, 0x00, 0x00, 0x00, // dim_size

        0x04, 0x00, 0x00, 0x00, // length of "test"
        't', 'e', 's', 't', // "test"
        0x0a, 0x00, 0x00, 0x00, // size
        0x14, 0x00, 0x00, 0x00, // stride

        0x05, 0x00, 0x00, 0x00, // length of "test2"
        't', 'e', 's', 't', '2', // "test2"
        0x14, 0x00, 0x00, 0x00, // size
        0x28, 0x00, 0x00, 0x00, // stride

        0x1c, 0x00, 0x00, 0x00 // data_offset
    };

    MultiArrayLayout *layout = MultiArrayLayout::deserialize(buffer, 41);
    EXPECT_EQ(layout->dim_size, (R_UInt32)2);
    EXPECT_EQ(layout->dim[0].label, "test");
    EXPECT_EQ(layout->dim[0].size, (R_UInt32)10);
    EXPECT_EQ(layout->dim[0].stride, (R_UInt32)20);
    EXPECT_EQ(layout->dim[1].label, "test2");
    EXPECT_EQ(layout->dim[1].size, (R_UInt32)20);
    EXPECT_EQ(layout->dim[1].stride, (R_UInt32)40);
    EXPECT_EQ(layout->data_offset, (R_UInt32)28);

    delete layout;
}