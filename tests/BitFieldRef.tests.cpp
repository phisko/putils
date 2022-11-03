// gtest
#include <gtest/gtest.h>

// putils
#include "BitFieldRef.hpp"

TEST(BitFieldRef, get) {
    const char i = 2;
    const putils::ConstBitFieldRef bitfield(&i);

    EXPECT_FALSE(bitfield.get(0));
    EXPECT_TRUE(bitfield.get(1));
    EXPECT_FALSE(bitfield.get(2));
    EXPECT_FALSE(bitfield.get(3));
}

TEST(BitFieldRef, get_byte_index) {
    const int i = 512;
    const putils::ConstBitFieldRef bitfield(&i);

    EXPECT_FALSE(bitfield.get(0));
    EXPECT_FALSE(bitfield.get(1));
    EXPECT_FALSE(bitfield.get(2));
    EXPECT_FALSE(bitfield.get(3));

    EXPECT_FALSE(bitfield.get(1, 0));
    EXPECT_TRUE(bitfield.get(1, 1));
    EXPECT_FALSE(bitfield.get(1, 2));
    EXPECT_FALSE(bitfield.get(1, 3));
}

TEST(BitFieldRef, set) {
    int i = 2;
    putils::BitFieldRef bitfield(&i);
    bitfield.set(0, true);
    EXPECT_EQ(i, 3);
    bitfield.set(1, false);
    EXPECT_EQ(i, 1);
}

TEST(BitFieldRef, set_byte_index) {
    int i = 2;
    putils::BitFieldRef bitfield(&i);
    bitfield.set(1, 0, true);
    EXPECT_EQ(i, 258);
    bitfield.set(0, 1, false);
    EXPECT_EQ(i, 256);
}

TEST(BitFieldRef, toggle) {
    int i = 2;
    putils::BitFieldRef bitfield(&i);
    bitfield.toggle(0);
    EXPECT_EQ(i, 3);
    bitfield.toggle(1);
    EXPECT_EQ(i, 1);
}

TEST(BitFieldRef, toggle_byte_index) {
    int i = 2;
    putils::BitFieldRef bitfield(&i);
    bitfield.toggle(1, 0);
    EXPECT_EQ(i, 258);
    bitfield.toggle(0, 1);
    EXPECT_EQ(i, 256);
}
