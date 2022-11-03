// gtest
#include <gtest/gtest.h>

// putils
#include "erase.hpp"

TEST(erase, found) {
    std::vector<int> vec{ 0, 1, 2, 3 };
    putils::erase(vec, 2);
    EXPECT_EQ(vec.size(), 3);

    const std::vector<int> expected{ 0, 1, 3 };
    for (size_t i = 0; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], expected[i]);
}

TEST(erase, notFound) {
    std::vector<int> vec{ 0, 1, 2, 3 };
    putils::erase(vec, 4);
    EXPECT_EQ(vec.size(), 4);

    const std::vector<int> expected{ 0, 1, 2, 3 };
    for (size_t i = 0; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], expected[i]);
}

TEST(erase, multiple) {
    std::vector<int> vec{ 0, 2, 1, 2, 3 };
    putils::erase(vec, 2);
    EXPECT_EQ(vec.size(), 4);

    const std::vector<int> expected{ 0, 1, 2, 3 };
    for (size_t i = 0; i < vec.size(); ++i)
        EXPECT_EQ(vec[i], expected[i]);
}
