// gtest
#include <gtest/gtest.h>

// putils
#include "putils/chop.hpp"

TEST(chop, start) {
	const auto str = putils::chop(" hello");
	EXPECT_EQ(str, "hello");
}

TEST(chop, end) {
	const auto str = putils::chop("hello ");
	EXPECT_EQ(str, "hello");
}

TEST(chop, quotes) {
	const auto str = putils::chop("\"hello\"");
	EXPECT_EQ(str, "hello");
}

TEST(chop, single_quotes) {
	const auto str = putils::chop("'hello'");
	EXPECT_EQ(str, "hello");
}

TEST(chop, all) {
	const auto str = putils::chop("  \"'hello'\"  ");
	EXPECT_EQ(str, "hello");
}