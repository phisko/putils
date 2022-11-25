// gtest
#include <gtest/gtest.h>

// putils
#include "putils/clamp.hpp"

TEST(clamp, types) {
    EXPECT_EQ(putils::clamp(100.f, 0.f, 1.f), 1.f);
    EXPECT_EQ(putils::clamp(100, 0, 1), 1);
    EXPECT_EQ(putils::clamp(100.0, 0.0, 1.5), 1.5);
}

TEST(clamp, min) {
    EXPECT_EQ(putils::clamp(-1, 0, 1), 0);
}

TEST(clamp, max) {
    EXPECT_EQ(putils::clamp(2, 0, 1), 1);
}

TEST(clamp, normal) {
    EXPECT_EQ(putils::clamp(1, 0, 2), 1);
}
