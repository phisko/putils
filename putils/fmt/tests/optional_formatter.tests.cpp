// gtest
#include <gtest/gtest.h>

// putils
#include "putils/fmt/formattable.hpp"
#include "putils/fmt/optional_formatter.hpp"

TEST(optional_formatter, null) {
	const auto str = fmt::format("{}", std::optional<int>());
	EXPECT_EQ(str, "null");
}

TEST(optional_formatter, not_null) {
	const auto str = fmt::format("{}", std::optional<int>(42));
	EXPECT_EQ(str, "(42)");
}

TEST(optional_formatter, formattable) {
	static_assert(putils::formattable<std::optional<int>>);
}