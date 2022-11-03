// gtest
#include <gtest/gtest.h>

// putils
#include "regex.hpp"

using namespace putils::regex;

TEST(regex, bool) {
    EXPECT_TRUE("hello" == std::regex("^h.*$"));
}

TEST(regex, match) {
    const auto match = ("hello" == std::make_pair(std::regex("^h(.*)$"), true));
    EXPECT_EQ(match.size(), 2);
    EXPECT_EQ(match[1], "ello");
}

TEST(regex, boolLiteral) {
    EXPECT_TRUE("hello" == "^h.*$"_r);
}

TEST(regex, matchLiteral) {
    const auto match = ("hello" == "^h(.*)$"_m);
    EXPECT_EQ(match.size(), 2);
    EXPECT_EQ(match[1], "ello");
}