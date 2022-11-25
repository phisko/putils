// gtest
#include <gtest/gtest.h>

// putils
#include "putils/forward_to.hpp"

static int func(int i) noexcept {
	return i;
}

TEST(forward_to, function) {
	const auto f = putils_forward_to(func);
	EXPECT_EQ(f(42), 42);
}

struct callable {
	int i = 0;

	int func(int j) const {
		return i + j;
	}
};

TEST(forward_to, global) {
	static const callable g{ 42 };

	const auto f = putils_forward_to(g.func);
	EXPECT_EQ(f(42), 84);
}

TEST(forward_to, ref) {
	callable g;
	g.i = 42;

	const auto f = putils_forward_to_ref(g, func);
	EXPECT_EQ(f(42), 84);
}

TEST(forward_to, ptr) {
	callable g;
	g.i = 42;

	const callable * ptr = &g;

	const auto f = putils_forward_to_ptr(ptr, func);
	EXPECT_EQ(f(42), 84);
}