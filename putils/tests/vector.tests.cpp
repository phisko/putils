// gtest
#include <gtest/gtest.h>

// putils
#include "putils/vector.hpp"

TEST(vector, ctor) {
    const putils::vector<int, 8> v;
    EXPECT_EQ(v.size(), 0);
}

TEST(vector, ctor_static_array) {
    const int arr[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    const putils::vector<int, 8> v(arr);
    EXPECT_EQ(v.size(), 8);
    for (int i = 0; i < 8; ++i)
        EXPECT_EQ(v[i], i);
}

TEST(vector, ctor_init_list) {
    const putils::vector<int, 8> v({ 0, 1, 2, 3 });
    EXPECT_EQ(v.size(), 4);
    for (int i = 0; i < 4; ++i)
        EXPECT_EQ(v[i], i);
}

TEST(vector, push_back) {
    putils::vector<int, 8> v;
    v.push_back(42);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);
}

TEST(vector, try_push_back) {
    putils::vector<int, 1> v;

    v.try_push_back(42);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);

    v.try_push_back(42);
    EXPECT_EQ(v.size(), 1);
}

TEST(vector, emplace_back) {
    putils::vector<int, 8> v;

    v.emplace_back(42);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);
}

TEST(vector, try_emplace_back) {
    putils::vector<int, 1> v;

    v.try_emplace_back(42);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);

    v.try_emplace_back(42);
    EXPECT_EQ(v.size(), 1);
}

TEST(vector, add) {
    putils::vector<int, 8> v;

    v.add(42);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);
}

TEST(vector, try_add) {
    putils::vector<int, 1> v;

    v.try_add(42);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);

    v.try_add(42);
    EXPECT_EQ(v.size(), 1);
}

TEST(vector, front) {
    const putils::vector<int, 8> v{0, 1, 2};
    EXPECT_EQ(v.front(), 0);
}

TEST(vector, first) {
    putils::vector<int, 8> v{0, 1, 2};
    EXPECT_EQ(v.first(), 0);
}

TEST(vector, back) {
    const putils::vector<int, 8> v{0, 1, 2};
    EXPECT_EQ(v.back(), 2);
}

TEST(vector, last) {
    putils::vector<int, 8> v{0, 1, 2};
    EXPECT_EQ(v.last(), 2);
}

TEST(vector, index) {
    const putils::vector<int, 8> v{0, 1, 2};
    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(v[i], i);
}

TEST(vector, at) {
    const putils::vector<int, 8> v{0, 1, 2};
    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(v.at(i), i);
}

TEST(vector, get) {
    putils::vector<int, 8> v{0, 1, 2};
    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(v.get(i), i);
}

TEST(vector, erase) {
    putils::vector<int, 8> v{0, 1, 2};
    v.erase(&v[1]);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 2);
}

TEST(vector, erase_until) {
    putils::vector<int, 8> v{0, 1, 2};
    v.erase(&v[1], v.end());
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 0);
}

TEST(vector, remove) {
    putils::vector<int, 8> v{0, 1, 2};
    v.remove(&v[1]);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 2);
}

TEST(vector, size) {
    putils::vector<int, 8> v{0, 1, 2};
    EXPECT_EQ(v.size(), 3);
    v.clear();
    EXPECT_EQ(v.size(), 0);
}

TEST(vector, empty) {
    putils::vector<int, 8> v{0, 1, 2};
    EXPECT_FALSE(v.empty());
    v.clear();
    EXPECT_TRUE(v.empty());
}

TEST(vector, full) {
    putils::vector<int, 3> v{0, 1, 2};
    EXPECT_TRUE(v.full());
    v.clear();
    EXPECT_FALSE(v.full());
}

TEST(vector, data) {
    const putils::vector<int, 3> v{0, 1, 2};
    EXPECT_TRUE(v.data() == &v[0]);
}

TEST(vector, resize) {
    putils::vector<int, 8> v{ 0, 1, 2 };
    v.resize(2);
    EXPECT_EQ(v.size(), 2);
}

TEST(vector, capacity) {
    const putils::vector<int, 8> v{ 0, 1, 2 };
    EXPECT_EQ(v.capacity(), 8);
}

TEST(vector, make_vector) {
    const auto v = putils::make_vector(0, 1, 2);
    EXPECT_EQ(v.size(), 3);
    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(v[i], i);
}

TEST(vector, is_vector) {
    static_assert(putils::is_vector<putils::vector<int, 8>>());
    static_assert(putils::is_vector<putils::vector<char, 8>>());

    struct S {};
    static_assert(putils::is_vector<putils::vector<S, 8>>());

    static_assert(!putils::is_vector<std::string>());
    static_assert(!putils::is_vector<std::vector<int>>());
}