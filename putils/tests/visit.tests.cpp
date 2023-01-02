// gtest
#include <gtest/gtest.h>

// stl
#include <variant>

// putils
#include "putils/visit.hpp"

TEST(overloaded, visit) {
	const std::variant<int, std::string> v = "hello";

	int res_int = 0;
	std::string res_str;
	std::visit(putils::overloaded{
				   [&](int i) {
					   res_int = i;
				   },
				   [&](const std::string & s) {
					   res_str = s;
				   } },
		v);
	EXPECT_EQ(res_int, 0);
	EXPECT_EQ(res_str, "hello");
}