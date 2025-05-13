#include <gtest/gtest.h>
#include "std_msg/Bool.hpp"

TEST(BoolTest, Serialize) {
    BoolMsg d(true);
    uint8_t buffer[sizeof(uint8_t)];
    size_t size = d.serialize(buffer, sizeof(buffer));

    uint8_t expected[] = {0x01};

    // expect the serialized size to be correct
    EXPECT_EQ(size, sizeof(uint8_t));

    // expect the serialized data to be correct
    for (size_t i = 0; i < sizeof(uint8_t); ++i) {
        EXPECT_EQ(buffer[i], expected[i]);
    }
}
TEST(BoolTest, Deserialize) {
    uint8_t buffer[sizeof(uint8_t)];
    uint8_t expected[] = {0x01};
    for (size_t i = 0; i < sizeof(uint8_t); ++i) {
        buffer[i] = expected[i];
    }

    BoolMsg *d = BoolMsg::deserialize(buffer, sizeof(buffer));

    EXPECT_EQ(d->value, true);

    delete d;
}