#include <gtest/gtest.h>
#include "builtin_interfaces/Duration.hpp"

TEST(DurationTest, Serialize) {
    DurationMsg d(0x12345678, 0x9abcdef0);
    uint8_t buffer[sizeof(uint32_t) * 2];
    size_t size = d.serialize(buffer, sizeof(buffer));

    uint32_t expected[] = {0x12345678,
                          0x9abcdef0};
    uint8_t *expected_bytes = reinterpret_cast<uint8_t *>(expected);

    // expect the serialized size to be correct
    EXPECT_EQ(size, sizeof(uint32_t) * 2);

    // expect the serialized data to be correct
    for (size_t i = 0; i < sizeof(uint32_t) * 2; ++i) {
        EXPECT_EQ(buffer[i], expected_bytes[i]);
    }
}

TEST(DurationTest, Deserialize) {
    uint8_t buffer[sizeof(uint32_t) * 2];
    uint32_t expected[] = {0x12345678,
                          0x9abcdef0};
    uint8_t *expected_bytes = reinterpret_cast<uint8_t *>(expected);
    for (size_t i = 0; i < sizeof(uint32_t) * 2; ++i) {
        buffer[i] = expected_bytes[i];
    }

    DurationMsg *d = DurationMsg::deserialize(buffer, sizeof(buffer));

    EXPECT_EQ(d->sec, 0x12345678);
    EXPECT_EQ(d->nanosec, 0x9abcdef0);

    delete d;
}