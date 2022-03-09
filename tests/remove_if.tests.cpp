#include <gtest/gtest.h>
#include "remove_if.hpp"

TEST(remove_if, notFound) {
    std::vector<int> v{ 0, 1, 2, 3 };
    putils::remove_if(v, [](int i) { return i == 4; });
    EXPECT_EQ(v, std::vector<int>({ 0, 1, 2, 3 }));
}

TEST(remove_if, found) {
    std::vector<int> v{ 0, 1, 2, 3 };
    putils::remove_if(v, [](int i) { return i == 1; });
    EXPECT_EQ(v, std::vector<int>({ 0, 2, 3 }));
}

TEST(remove_if, multiple) {
    std::vector<int> v{ 0, 1, 1, 2, 3 };
    putils::remove_if(v, [](int i) { return i == 1; });
    EXPECT_EQ(v, std::vector<int>({ 0, 2, 3 }));
}