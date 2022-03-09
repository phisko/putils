#include <gtest/gtest.h>
#include "casts.hpp"

struct A {
    virtual ~A() = default;
};

struct B : A {
};

TEST(casts, static_unique_ptr_cast) {
    auto ptr = std::make_unique<A>();
    const auto ptr2 = putils::static_unique_ptr_cast<B>(std::move(ptr));
    EXPECT_NE(ptr2, nullptr);

    ptr = std::make_unique<B>();
    const auto ptr3 = putils::static_unique_ptr_cast<A>(std::move(ptr));
    EXPECT_NE(ptr2, nullptr);
}

TEST(casts, dynamic_unique_ptr_cast) {
    auto ptr = std::make_unique<A>();
    const auto ptr2 = putils::dynamic_unique_ptr_cast<B>(std::move(ptr));
    EXPECT_EQ(ptr2, nullptr);

    ptr.reset(new B);
    const auto ptr3 = putils::dynamic_unique_ptr_cast<B>(std::move(ptr));
    EXPECT_NE(ptr3, nullptr);
}