// gtest
#include <gtest/gtest.h>

// putils
#include "putils/sign.hpp"

TEST(sign, positive) {
    EXPECT_EQ(putils::sign(42), 1);
}

TEST(sign, negative) {
    EXPECT_EQ(putils::sign(-42), -1);
}

TEST(sign, zero) {
    EXPECT_EQ(putils::sign(0), 0);
}
