// gtest
#include <gtest/gtest.h>

// stl
#include <vector>

// putils
#include "putils/scn/range_scanner.hpp"
#include "putils/scn/scannable.hpp"

TEST(range_scanner, vector_empty) {
	std::vector<int> v;
	const auto result = scn::scan("[]", "{}", v);
	EXPECT_TRUE(result);
	EXPECT_TRUE(v.empty());
}

TEST(range_scanner, vector) {
	std::vector<int> v;
	const auto result = scn::scan("[0, 42, 84]", "{}", v);
	EXPECT_TRUE(result);
	const std::vector<int> expected{ 0, 42, 84 };
	EXPECT_EQ(v, expected);
}

TEST(range_scanner, vector_string) {
	std::vector<std::string> v;
	const auto result = scn::scan("[hello, foo, bar]", "{}", v);
	EXPECT_TRUE(result);
	const std::vector<std::string> expected{ "hello", "foo", "bar" };
	EXPECT_EQ(v, expected);
}

TEST(range_scanner, map_empty) {
	std::map<int, std::string> m;
	const auto result = scn::scan("{}", "{}", m);
	EXPECT_TRUE(result);
	EXPECT_TRUE(m.empty());
}

TEST(range_scanner, map) {
	std::map<int, std::string> m;
	const auto result = scn::scan("{ 0: zero, 40: forty }", "{}", m);
	EXPECT_TRUE(result);
	const std::map<int, std::string> expected {
		{ 0, "zero" },
		{ 40, "forty" }
	};
	EXPECT_EQ(m, expected);
}

TEST(range_scanner, set_empty) {
	std::set<int> s;
	const auto result = scn::scan("{}", "{}", s);
	EXPECT_TRUE(result);
	EXPECT_TRUE(s.empty());
}

TEST(range_scanner, set) {
	std::set<int> s;
	const auto result = scn::scan("{ 0, 42, 84 }", "{}", s);
	EXPECT_TRUE(result);
	const std::set<int> expected { 0, 42, 84 };
	EXPECT_EQ(s, expected);
}

TEST(range_scanner, scannable) {
	static_assert(putils::scannable<std::vector<int>>);
}