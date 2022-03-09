#include <gtest/gtest.h>
#include "concatenate.hpp"

TEST(concatenate, empty) {
    EXPECT_TRUE(putils::concatenate().empty());
}

TEST(concatenate, single) {
    EXPECT_EQ(putils::concatenate("hi"), "hi");
}

TEST(concatenate, strings) {
    EXPECT_EQ(putils::concatenate("hi", "hello"), "hihello");
}

TEST(concatenate, stringInt) {
    EXPECT_EQ(putils::concatenate("hi", 42, 42.5f), "hi4242.5");
}