// gtest
#include <gtest/gtest.h>

// putils
#include "putils/casts.hpp"

struct a {
	virtual ~a() = default;
};

struct b : a {
};

TEST(casts, static_unique_ptr_cast) {
	auto ptr = std::make_unique<a>();
	const auto ptr2 = putils::static_unique_ptr_cast<b>(std::move(ptr));
	EXPECT_NE(ptr2, nullptr);

	ptr = std::make_unique<b>();
	const auto ptr3 = putils::static_unique_ptr_cast<a>(std::move(ptr));
	EXPECT_NE(ptr2, nullptr);
}

TEST(casts, dynamic_unique_ptr_cast) {
	auto ptr = std::make_unique<a>();
	const auto ptr2 = putils::dynamic_unique_ptr_cast<b>(std::move(ptr));
	EXPECT_EQ(ptr2, nullptr);

	ptr.reset(new b);
	const auto ptr3 = putils::dynamic_unique_ptr_cast<b>(std::move(ptr));
	EXPECT_NE(ptr3, nullptr);
}