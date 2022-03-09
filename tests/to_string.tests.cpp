#include <gtest/gtest.h>
#include "to_string.hpp"

TEST(toString, int) {
    EXPECT_EQ(putils::toString(42), "42");
}

TEST(toString, string) {
    EXPECT_EQ(putils::toString("42"), "42");
}

TEST(toString, bool) {
    EXPECT_EQ(putils::toString(false), "false");
}

TEST(toString, stream) {
    std::stringstream s;
    s << 42;
    EXPECT_EQ(putils::toString(s), "42");
}

TEST(parse, int) {
    int i = 0;
    putils::parse(i, "42");
    EXPECT_EQ(i, 42);

    EXPECT_EQ(putils::parse<int>("42"), 42);
}

TEST(parse, string) {
    std::string s;
    putils::parse(s, "42");
    EXPECT_EQ(s, "42");

    EXPECT_EQ(putils::parse<std::string>("42"), "42");
}

TEST(parse, bool) {
    bool b = true;
    putils::parse(b, "false");
    EXPECT_EQ(b, false);

    EXPECT_EQ(putils::parse<bool>("false"), false);
}