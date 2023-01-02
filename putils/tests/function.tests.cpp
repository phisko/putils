// gtest
#include <gtest/gtest.h>

// meta
#include "putils/meta/traits/function_arguments.hpp"
#include "putils/meta/traits/function_return_type.hpp"
#include "putils/meta/traits/function_signature.hpp"
#include "putils/meta/traits/is_function.hpp"

// putils
#include "putils/function.hpp"

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

TEST(function, function_signature) {
	const putils::function<bool(int, double), 64> f;
	static_assert(std::is_same<bool (*)(int, double), putils::function_signature<decltype(f)>>());
	SUCCEED();
}

TEST(function, is_function) {
	const putils::function<void(int, double), 64> f;
	static_assert(putils::is_function<decltype(f)>());
	SUCCEED();
}

TEST(function, function_arguments) {
	const putils::function<void(int, double), 64> f;
	static_assert(std::is_same<std::tuple<int, double>, putils::function_arguments<decltype(f)>>());
	SUCCEED();
}

TEST(function, function_return_type) {
	const putils::function<bool(int, double), 64> f;
	static_assert(std::is_same<bool, putils::function_return_type<decltype(f)>>());
	SUCCEED();
}
