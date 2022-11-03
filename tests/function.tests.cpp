// gtest
#include <gtest/gtest.h>

// putils
#include "function.hpp"

static int test(int i) { return i; }

TEST(function, static) {
    const putils::function<int(int), 40> f(test); // 40 is the smallest acceptable size (32 for function pointer, 8 for vtable)
    EXPECT_EQ(f(42), 42);
}

TEST(function, lambda) {
    const auto l = [](int i) { return i; };
    const putils::function<int(int), 40> f(l);
    EXPECT_EQ(f(42), 42);
}

TEST(function, lambdaCapture) {
    int j = 0;
    const auto l = [&](int i) {
        j = i;
        return i;
    };

    const putils::function<int(int), 40> f(l);
    EXPECT_EQ(f(42), 42);
    EXPECT_EQ(j, 42);
}
