#include <gtest/gtest.h>
#include "same_sign.hpp"

TEST(same_sign, twoPositives) {
    EXPECT_TRUE(putils::sameSign(1, 2));
}

TEST(same_sign, twoNegatives) {
    EXPECT_TRUE(putils::sameSign(-1, -2));
}

TEST(same_sign, twoZeros) {
    EXPECT_TRUE(putils::sameSign(0, 0));
}

TEST(same_sign, zeroPositve) {
    EXPECT_TRUE(putils::sameSign(0, 1));
}

TEST(same_sign, zeroNegative) {
    EXPECT_TRUE(putils::sameSign(0, -1));
}

TEST(same_sign, positiveNegative) {
    EXPECT_FALSE(putils::sameSign(1, -1));
}
