#include <gtest/gtest.h>
#include "scoped_setter.hpp"

TEST(scoped_setter, set) {
    int i = 0;
    {
        const auto _ = putils::setForScope(i, 42);
        EXPECT_EQ(i, 42);
    }
    EXPECT_EQ(i, 0);
}