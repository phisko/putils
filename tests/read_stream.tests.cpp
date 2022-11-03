// gtest
#include <gtest/gtest.h>

// putils
#include "read_stream.hpp"

TEST(read_stream, empty) {
    std::stringstream s;
    EXPECT_EQ(putils::read_stream(s), "");
}

TEST(read_stream, oneWord) {
    const char * str = "hello";
    std::stringstream s(str);
    EXPECT_EQ(putils::read_stream(s), str);
}

TEST(read_stream, twoWords) {
    const char * str = "hello world";
    std::stringstream s(str);
    EXPECT_EQ(putils::read_stream(s), str);
}

TEST(read_stream, twoLines) {
    const char * str = R"(
hello
world
)";
    std::stringstream s(str);
    EXPECT_EQ(putils::read_stream(s), str);
}

TEST(read_stream, skippedLine) {
    const char * str = R"(
hello

world
)";
    std::stringstream s(str);
    EXPECT_EQ(putils::read_stream(s), str);
}
