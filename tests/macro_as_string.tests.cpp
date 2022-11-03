// gtest
#include <gtest/gtest.h>

// putils
#include "macro_as_string.hpp"

TEST(macro_as_string, macro_as_string) {
#define macro 42
    static_assert(putils_macro_as_string(macro) == std::string_view("42"));
#undef macro
}