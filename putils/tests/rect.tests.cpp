// gtest
#include <gtest/gtest.h>

// putils
#include "putils/rect.hpp"

TEST(rect, compare) {
    const putils::rect2i r{ { 0, 0 }, { 1, 1 } };
    const putils::rect2f r2{ { 0.f, 0.f }, { 1.f, 1.f } };
    EXPECT_TRUE(r == r2);

    const putils::rect2i r3{ { 1, 1 }, { 1, 1 } };
    EXPECT_TRUE(r != r3);
}