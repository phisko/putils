// gtest
#include <gtest/gtest.h>

// putils
#include "epsilon.hpp"

TEST(epsilon, true) {
    EXPECT_TRUE(putils::epsilonEquals(1.f, 1.00000000001f));
}

TEST(epsilon, false) {
    EXPECT_FALSE(putils::epsilonEquals(1.f, 2.f));
}
