// gtest
#include <gtest/gtest.h>

// putils
#include "putils/fmt/fmt.hpp"
#include "putils/string.hpp"
#include "putils/vector.hpp"

struct empty {};
#define refltype empty
putils_reflection_info {
	putils_reflection_attributes();
};
#undef refltype

TEST(reflectible_formatter, empty) {
	const auto str = fmt::format("{}", empty{});
	EXPECT_EQ(str, "{}");
}

struct values {
	int i = 42;
	double d = 84;
	std::string s = "hello";
	putils::string<64> ps = "foo";
	putils::vector<int, 64> pv{ 0, 42, 84 };
};
#define refltype values
putils_reflection_info {
	putils_reflection_attributes(
		putils_reflection_attribute(i),
		putils_reflection_attribute(d),
		putils_reflection_attribute(s),
		putils_reflection_attribute(ps),
		putils_reflection_attribute(pv)
	)
};
#undef refltype

TEST(reflectible_formatter, values) {
	const auto str = fmt::format("{}", values{});
	EXPECT_EQ(str, "{i: 42, d: 84, s: hello, ps: foo, pv: [0, 42, 84]}");
}

TEST(reflectible_formatter, formattable) {
	static_assert(putils::formattable<values>);
}