// gtest
#include <gtest/gtest.h>

// putils
#include "putils/epsilon.hpp"

TEST(epsilon, true) {
    EXPECT_TRUE(putils::epsilon_equals(1.f, 1.00000000001f));
}

TEST(epsilon, false) {
    EXPECT_FALSE(putils::epsilon_equals(1.f, 2.f));
}
