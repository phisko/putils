// gtest
#include <gtest/gtest.h>

// putils
#include "putils/scoped_setter.hpp"

TEST(scoped_setter, set) {
	int i = 0;
	{
		const auto _ = putils::set_for_scope(i, 42);
		EXPECT_EQ(i, 42);
	}
	EXPECT_EQ(i, 0);
}