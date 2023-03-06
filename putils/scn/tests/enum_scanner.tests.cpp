// gtest
#include <gtest/gtest.h>

// putils
#include "putils/scn/enum_scanner.hpp"
#include "putils/scn/scannable.hpp"

TEST(enum_scanner, value) {
	enum class e { a, b };
	e value = e::b;
	const auto result = scn::scan("a", "{}", value);
	EXPECT_TRUE(result);
	EXPECT_EQ(value, e::a);
}

TEST(enum_scanner, empty) {
	enum e {};
	e value = e(0);
	const auto result = scn::scan("42", "{}", value);
	EXPECT_TRUE(result);
	EXPECT_EQ(value, e(42));
}

TEST(enum_scanner, scannable) {
	enum e{};
	static_assert(putils::scannable<e>);
}