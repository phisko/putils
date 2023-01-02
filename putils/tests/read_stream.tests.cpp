// gtest
#include <gtest/gtest.h>

// putils
#include "putils/read_stream.hpp"

TEST(read_stream, empty) {
	std::stringstream s;
	EXPECT_EQ(putils::read_stream(s), "");
}

TEST(read_stream, one_word) {
	const char * str = "hello";
	std::stringstream s(str);
	EXPECT_EQ(putils::read_stream(s), str);
}

TEST(read_stream, two_words) {
	const char * str = "hello world";
	std::stringstream s(str);
	EXPECT_EQ(putils::read_stream(s), str);
}

TEST(read_stream, two_lines) {
	const char * str = R"(
hello
world
)";
	std::stringstream s(str);
	EXPECT_EQ(putils::read_stream(s), str);
}

TEST(read_stream, skipped_line) {
	const char * str = R"(
hello

world
)";
	std::stringstream s(str);
	EXPECT_EQ(putils::read_stream(s), str);
}
