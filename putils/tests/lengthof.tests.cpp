// gtest
#include <gtest/gtest.h>

// putils
#include "putils/lengthof.hpp"

TEST(lengthof, single) {
    static_assert(putils::lengthof<int>() == 1);
}

TEST(lengthof, array) {
    int i[42];
    static_assert(putils::lengthof(i) == 42);
}