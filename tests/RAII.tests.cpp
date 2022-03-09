#include <gtest/gtest.h>
#include "RAII.hpp"

static void release(int * & val) {
    *val = *val + 1;
}

TEST(RAII, dtor) {
    int i = 0;
    {
        putils::RAII<int*, release> raii(&i);
    }
    EXPECT_EQ(i, 1);
}

TEST(RAII, move) {
    int i = 0;
    {
        putils::RAII<int*, release> raii(&i);
        const auto moved = std::move(raii);
    }
    EXPECT_EQ(i, 1); // release should be called only once
}

TEST(RAII, assign) {
    int i = 0;
    int j = 0;
    {
        putils::RAII<int*, release> raii(&i);
        raii = &j;
    }
    EXPECT_EQ(i, 1);
    EXPECT_EQ(j, 1);
}

TEST(RAII, get) {
    int i = 0;
    putils::RAII<int*, release> raii(&i);
    EXPECT_EQ(raii.get(), &i);
}
