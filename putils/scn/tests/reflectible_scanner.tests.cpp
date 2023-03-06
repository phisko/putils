// gtest
#include <gtest/gtest.h>

// putils
#include "putils/scn/scn.hpp"

struct values {
	int i = 42;
	double d = 84;
	std::string s = "hello";
	std::string s2 = "hello";
};

#define refltype values
putils_reflection_info {
	putils_reflection_attributes(
		putils_reflection_attribute(i),
		putils_reflection_attribute(d),
		putils_reflection_attribute(s),
		putils_reflection_attribute(s2)
	)
};
#undef refltype

TEST(reflectible_scanner, values) {
	values v;
	const auto result = scn::scan_default("{ i: 84, d: 168, s: foo, s2: bar }", v);
	EXPECT_TRUE(result);
	EXPECT_EQ(v.i, 84);
	EXPECT_EQ(v.d, 168);
	EXPECT_EQ(v.s, "foo");
	EXPECT_EQ(v.s2, "bar");
}

struct vectors {
	std::vector<int> iv;
	std::vector<std::string> sv;
	std::vector<std::vector<int>> ivv;
};

#define refltype vectors
putils_reflection_info {
	putils_reflection_attributes(
		putils_reflection_attribute(iv),
		putils_reflection_attribute(sv),
		putils_reflection_attribute(ivv)
	)
};
#undef refltype

TEST(reflectible_scanner, vectors) {
	vectors v;
	const auto result = scn::scan_default("{ iv: [ 0, 42, 84 ], sv: [ hello, foo, bar ], ivv: [ [0, 1], [2, 3] ] }", v);
	EXPECT_TRUE(result);

	const std::vector<int> iv{ 0, 42, 84 };
	EXPECT_EQ(v.iv, iv);

	const std::vector<std::string> sv{ "hello", "foo", "bar" };
	EXPECT_EQ(v.sv, sv);

	const std::vector<std::vector<int>> ivv{ { 0, 1 }, { 2, 3 } };
	EXPECT_EQ(v.ivv, ivv);
}

struct maps {
	std::map<int, std::string> is;
	std::unordered_map<std::string, std::string> ss;
};

#define refltype maps
putils_reflection_info {
	putils_reflection_attributes(
		putils_reflection_attribute(is),
		putils_reflection_attribute(ss)
	)
};
#undef refltype

TEST(reflectible_scanner, maps) {
	maps m;
	const auto result = scn::scan_default("{ is: { 0: zero, 40: forty }, ss: { one: 1, two: 2, foo: bar } }", m);
	EXPECT_TRUE(result);

	const std::map<int, std::string> is{
		{ 0, "zero" },
		{ 40, "forty" }
	};
	EXPECT_EQ(m.is, is);

	const std::unordered_map<std::string, std::string> ss{
		{ "one", "1" },
		{ "two", "2" },
		{ "foo", "bar" }
	};
	EXPECT_EQ(m.ss, ss);
}

TEST(reflectible_scanner, scannable) {
	static_assert(putils::scannable<maps>);
}