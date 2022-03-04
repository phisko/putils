#include <gtest/gtest.h>
#include "reflection_helpers/json_helper.hpp"

namespace {
    struct Reflectible {
        int i = 42;
        std::string s = "hello";
    };
}

#define refltype Reflectible
putils_reflection_info {
    putils_reflection_attributes(
            putils_reflection_attribute(i),
            putils_reflection_attribute(s)
    );
};
#undef refltype

TEST(json_helper, toJSON_int) {
    EXPECT_EQ(putils::reflection::toJSON(42), putils::json(42));
}

TEST(json_helper, toJSON_string) {
    EXPECT_EQ(putils::reflection::toJSON("42"), putils::json("42"));
}

TEST(json_helper, toJSON_std_string) {
    EXPECT_EQ(putils::reflection::toJSON(std::string("42")), putils::json("42"));
}

TEST(json_helper, toJSON_array) {
    const int obj[] = { 0, 1, 2 };
    EXPECT_EQ(putils::reflection::toJSON(obj), putils::json::parse("[0,1,2]"));
}

TEST(json_helper, toJSON_vector) {
    const std::vector<int> obj = { 0, 1, 2 };
    EXPECT_EQ(putils::reflection::toJSON(obj), putils::json::parse("[0,1,2]"));
}

TEST(json_helper, toJSON_map) {
    const std::map<std::string, int> obj = {
        { "one", 1 },
        { "two", 2 }
    };
    EXPECT_EQ(putils::reflection::toJSON(obj), putils::json::parse(R"(
{
    "one": 1,
    "two": 2
}
)"));
}

TEST(json_helper, toJSON_unordered_map) {
    const std::unordered_map<std::string, int> obj = {
            { "one", 1 },
            { "two", 2 }
    };
    EXPECT_EQ(putils::reflection::toJSON(obj), putils::json::parse(R"(
{
    "one": 1,
    "two": 2
}
)"));
}

TEST(json_helper, toJSON_reflection) {
    EXPECT_EQ(putils::reflection::toJSON(Reflectible{}), putils::json::parse(R"({"i":42,"s":"hello"})"));
}

TEST(json_helper, fromJSON_int) {
    int i = 0;
    putils::reflection::fromJSON(42, i);
    EXPECT_EQ(i, 42);
}

TEST(json_helper, fromJSON_string) {
    char buff[32];
    putils::reflection::fromJSON("42", buff);
    EXPECT_STREQ(buff, "42");
}

TEST(json_helper, fromJSON_std_string) {
    std::string s;
    putils::reflection::fromJSON("42", s);
    EXPECT_EQ(s, "42");
}

TEST(json_helper, fromJSON_array) {
    int obj[3] = { -1, -1, -1 };
    putils::reflection::fromJSON(putils::json::parse("[0,1,2]"), obj);
    for (int i = 0; i < putils::lengthof(obj); ++i)
        EXPECT_EQ(obj[i], i);
}

TEST(json_helper, fromJSON_vector) {
    std::vector<int> obj;
    putils::reflection::fromJSON(putils::json::parse("[0,1,2]"), obj);
    EXPECT_EQ(obj.size(), 3);
    for (int i = 0; i < obj.size(); ++i)
        EXPECT_EQ(obj[i], i);
}

TEST(json_helper, fromJSON_map) {
    std::map<std::string, int> obj;
    putils::reflection::fromJSON(putils::json::parse(R"(
{
    "one": 1,
    "two": 2
}
)"), obj);
    EXPECT_EQ(obj.size(), 2);
    EXPECT_EQ(obj["one"], 1);
    EXPECT_EQ(obj["two"], 2);
}

TEST(json_helper, fromJSON_unordered_map) {
    std::unordered_map<std::string, int> obj;
    putils::reflection::fromJSON(putils::json::parse(R"(
{
    "one": 1,
    "two": 2
}
)"), obj);
    EXPECT_EQ(obj.size(), 2);
    EXPECT_EQ(obj["one"], 1);
    EXPECT_EQ(obj["two"], 2);
}

TEST(json_helper, fromJSON_reflection) {
    Reflectible obj;
    putils::reflection::fromJSON(putils::json::parse(R"({"i":-42,"s":"hi"})"), obj);
    EXPECT_EQ(obj.i, -42);
    EXPECT_EQ(obj.s, "hi");
}
