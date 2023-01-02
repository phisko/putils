// gtest
#include <gtest/gtest.h>

// putils
#include "putils/to_string.hpp"

TEST(to_string, int) {
	EXPECT_EQ(putils::to_string(42), "42");
}

TEST(to_string, string) {
	EXPECT_EQ(putils::to_string("42"), "42");
}

TEST(to_string, bool) {
	EXPECT_EQ(putils::to_string(false), "false");
}

TEST(to_string, stream) {
	std::stringstream s;
	s << 42;
	EXPECT_EQ(putils::to_string(s), "42");
}

TEST(to_string, enum) {
	enum E {
		A,
		B
	};
	const E e = E::A;
	EXPECT_EQ(putils::to_string(e), "A");
}

TEST(parse, int) {
	int i = 0;
	putils::parse(i, "42");
	EXPECT_EQ(i, 42);

	EXPECT_EQ(putils::parse<int>("42"), 42);
}

TEST(parse, string) {
	std::string s;
	putils::parse(s, "42");
	EXPECT_EQ(s, "42");

	EXPECT_EQ(putils::parse<std::string>("42"), "42");
}

TEST(parse, bool) {
	bool b = true;
	putils::parse(b, "false");
	EXPECT_EQ(b, false);

	EXPECT_EQ(putils::parse<bool>("false"), false);
}

TEST(parse, enum) {
	enum class E {
		A,
		B
	};

	E e = E::B;
	putils::parse(e, "A");
	EXPECT_EQ(e, E::A);

	EXPECT_EQ(putils::parse<E>("B"), E::B);
}