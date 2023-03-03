// gtest
#include <gtest/gtest.h>

// putils
#include "putils/reflection_helpers/enum_formatter.hpp"

TEST(enum_formatter, value) {
	enum class e {
		a,
		b
	};

	const auto str = fmt::format("{}", e::b);
	EXPECT_EQ(str, "b");
}

TEST(enum_formatter, empty) {
	enum e {};
	const auto str = fmt::format("{}", e(42));
	EXPECT_EQ(str, "42");
}