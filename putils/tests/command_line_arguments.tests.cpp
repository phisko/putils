// gtest
#include <gtest/gtest.h>

// putils
#include "putils/command_line_arguments.hpp"

TEST(command_line_arguments, to_argument_vector) {
	const std::vector<const char *> args = {
		"foo",
		"bar"
	};

	const auto parsed = putils::to_argument_vector(int(args.size()), args.data());
	EXPECT_EQ(parsed.size(), args.size());
	for (size_t i = 0; i < parsed.size(); ++i)
		EXPECT_EQ(parsed[i], args[i]);
}

struct options {
	std::string s1;
	std::string s2;
	float f1 = 0.f;
	float f2 = 0.f;
};

#define refltype options
putils_reflection_info{
	putils_reflection_attributes(
		putils_reflection_attribute(s1, putils_reflection_metadata("flag", "s")),
		putils_reflection_attribute(s2),
		putils_reflection_attribute(f1),
		putils_reflection_attribute(f2))
};
#undef refltype

TEST(command_line_arguments, parse_one_space) {
	const std::vector<std::string_view> args{
		"--s1",
		"hello",
	};
	const auto parsed = putils::parse_arguments<options>(std::span(args));
	EXPECT_EQ(parsed.s1, "hello");
}

TEST(command_line_arguments, parse_one_assign) {
	const std::vector<std::string_view> args{
		"--s1=hello",
	};
	const auto parsed = putils::parse_arguments<options>(std::span(args));
	EXPECT_EQ(parsed.s1, "hello");
}

TEST(command_line_arguments, parse_multiple) {
	const std::vector<std::string_view> args{
		"--s1",
		"hello",
		"--s2=hi",
		"--f1",
		"42.5",
		"--f2",
		"-84",
	};
	const auto parsed = putils::parse_arguments<options>(std::span(args));
	EXPECT_EQ(parsed.s1, "hello");
	EXPECT_EQ(parsed.s2, "hi");
	EXPECT_EQ(parsed.f1, 42.5f);
	EXPECT_EQ(parsed.f2, -84.f);
}

TEST(command_line_arguments, flag_metadata) {
	const std::vector<std::string_view> args{
		"-s",
		"hello",
	};
	const auto parsed = putils::parse_arguments<options>(std::span(args));
	EXPECT_EQ(parsed.s1, "hello");
}
