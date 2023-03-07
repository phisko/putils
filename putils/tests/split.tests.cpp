// gtest
#include <gtest/gtest.h>

// putils
#include "putils/split.hpp"

TEST(split, empty) {
	const auto input = "";
	const char delim = '/';
	const std::vector<std::string_view> expected = {};

	EXPECT_EQ(putils::split(input, delim), expected);
}

TEST(split, no_delim) {
	const auto input = "foo";
	const char delim = '/';
	const std::vector<std::string_view> expected = { "foo" };

	EXPECT_EQ(putils::split(input, delim), expected);
}

TEST(split, one_delim) {
	const auto input = "foo/bar";
	const char delim = '/';
	const std::vector<std::string_view> expected = { "foo", "bar" };

	EXPECT_EQ(putils::split(input, delim), expected);
}

TEST(split, two_delim) {
	const auto input = "foo/bar/hello";
	const char delim = '/';
	const std::vector<std::string_view> expected = { "foo", "bar", "hello" };

	EXPECT_EQ(putils::split(input, delim), expected);
}

TEST(split, space_delim) {
	const auto input = "foo bar hello";
	const char delim = ' ';
	const std::vector<std::string_view> expected = { "foo", "bar", "hello" };

	EXPECT_EQ(putils::split(input, delim), expected);
}
