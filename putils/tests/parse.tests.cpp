// gtest
#include <gtest/gtest.h>

// putils
#include "putils/parse.hpp"

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

TEST(parse, empty_enum) {
	enum class E {};

	E e = E(42);
	putils::parse(e, "84");
	EXPECT_EQ(e, E(84));

	EXPECT_EQ(putils::parse<E>("42"), E(42));
}