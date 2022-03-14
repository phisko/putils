#include <gtest/gtest.h>
#include "command_line_arguments.hpp"

struct Args {
    std::string s1;
    std::string s2;
    float f1 = 0.f;
    float f2 = 0.f;
};

#define refltype Args
putils_reflection_info{
    putils_reflection_attributes(
        putils_reflection_attribute(s1, putils_reflection_metadata("flag", "s")),
        putils_reflection_attribute(s2),
        putils_reflection_attribute(f1),
        putils_reflection_attribute(f2)
    )
};
#undef refltype

TEST(command_line_arguments, parseOneSpace) {
    const std::vector<const char *> args {
        "--s1", "hello",
    };
    const auto parsed = putils::parseArguments<Args>(args.size(), args.data());
    EXPECT_EQ(parsed.s1, "hello");
}

TEST(command_line_arguments, parseOneAssign) {
    const std::vector<const char *> args {
        "--s1=hello",
    };
    const auto parsed = putils::parseArguments<Args>(args.size(), args.data());
    EXPECT_EQ(parsed.s1, "hello");
}

TEST(command_line_arguments, parseMultiple) {
    const std::vector<const char *> args {
        "--s1", "hello",
        "--s2=hi",
        "--f1", "42.5",
        "--f2", "-84",
    };
    const auto parsed = putils::parseArguments<Args>(args.size(), args.data());
    EXPECT_EQ(parsed.s1, "hello");
    EXPECT_EQ(parsed.s2, "hi");
    EXPECT_EQ(parsed.f1, 42.5f);
    EXPECT_EQ(parsed.f2, -84.f);
}

TEST(command_line_arguments, flagMetadata) {
    const std::vector<const char *> args {
        "-s", "hello",
    };
    const auto parsed = putils::parseArguments<Args>(args.size(), args.data());
    EXPECT_EQ(parsed.s1, "hello");
}