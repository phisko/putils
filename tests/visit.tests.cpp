#include <gtest/gtest.h>
#include <variant>
#include "visit.hpp"

TEST(overloaded, visit) {
    const std::variant<int, std::string> v = "hello";

    int resInt = 0;
    std::string resStr;
    std::visit(putils::overloaded{
            [&](int i) {
                resInt = i;
            },
            [&](const std::string & s) {
                resStr = s;
            }
    }, v);
    EXPECT_EQ(resInt, 0);
    EXPECT_EQ(resStr, "hello");
}