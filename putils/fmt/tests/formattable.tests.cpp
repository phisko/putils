// gtest
#include <gtest/gtest.h>

// putils
#include "putils/fmt/formattable.hpp"

TEST(formattable, false) {
	enum e{};
	static_assert(!putils::formattable<e>);
}

TEST(formattable, int) {
	static_assert(putils::formattable<int>);
}

TEST(formattable, c_string) {
	static_assert(putils::formattable<const char *>);
}

TEST(formattable, string) {
	static_assert(putils::formattable<std::string>);
}