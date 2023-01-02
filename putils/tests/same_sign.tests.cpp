// gtest
#include <gtest/gtest.h>

// putils
#include "putils/same_sign.hpp"

TEST(same_sign, two_positives) {
	EXPECT_TRUE(putils::same_sign(1, 2));
}

TEST(same_sign, two_negatives) {
	EXPECT_TRUE(putils::same_sign(-1, -2));
}

TEST(same_sign, two_zeros) {
	EXPECT_TRUE(putils::same_sign(0, 0));
}

TEST(same_sign, zero_positve) {
	EXPECT_TRUE(putils::same_sign(0, 1));
}

TEST(same_sign, zero_negative) {
	EXPECT_TRUE(putils::same_sign(0, -1));
}

TEST(same_sign, positive_negative) {
	EXPECT_FALSE(putils::same_sign(1, -1));
}
