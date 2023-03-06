// gtest
#include <gtest/gtest.h>

// putils
#include "putils/scn/scannable.hpp"

TEST(scannable, false) {
	enum e{};
	static_assert(!putils::scannable<e>);
}

TEST(scannable, int) {
	static_assert(putils::scannable<int>);
}

TEST(scannable, c_string) {
	static_assert(!putils::scannable<const char *>);
}

TEST(scannable, string) {
	static_assert(putils::scannable<std::string>);
}
