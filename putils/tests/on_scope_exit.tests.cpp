// gtest
#include <gtest/gtest.h>

// putils
#include "putils/on_scope_exit.hpp"

TEST(on_scope_exit, on_scope_exit) {
    int i = 0;
    {
        const auto tmp = putils::on_scope_exit([&] { i = 42; });
    }
    EXPECT_EQ(i, 42);
}