// gtest
#include <gtest/gtest.h>

// putils
#include "putils/scn/optional_scanner.hpp"
#include "putils/scn/scannable.hpp"

TEST(optional_scanner, null) {
	std::optional<int> o(42);
	const auto result = scn::scan("null", "{}", o);
	EXPECT_TRUE(result);
	EXPECT_EQ(o, std::nullopt);
}

TEST(optional_scanner, not_null) {
	std::optional<int> o;
	const auto result = scn::scan("(42)", "{}", o);
	EXPECT_TRUE(result);
	EXPECT_NE(o, std::nullopt);
	EXPECT_EQ(*o, 42);
}

TEST(optional_scanner, scannable) {
	static_assert(putils::scannable<std::optional<int>>);
}