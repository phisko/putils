#include <gtest/gtest.h>
#include "on_scope_exit.hpp"

TEST(on_scope_exit, onScopeExit) {
    int i = 0;
    {
        const auto tmp = putils::onScopeExit([&] { i = 42; });
    }
    EXPECT_EQ(i, 42);
}