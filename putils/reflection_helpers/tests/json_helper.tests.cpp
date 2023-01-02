// gtest
#include <gtest/gtest.h>

// putils
#include "putils/reflection_helpers/json_helper.hpp"

namespace {
	struct reflectible {
		int i = 42;
		std::string s = "hello";
	};
}

#define refltype reflectible
putils_reflection_info {
	putils_reflection_attributes(
		putils_reflection_attribute(i),
		putils_reflection_attribute(s));
};
#undef refltype

TEST(json_helper, to_json_int) {
	EXPECT_EQ(putils::reflection::to_json(42), nlohmann::json(42));
}

TEST(json_helper, to_json_string) {
	EXPECT_EQ(putils::reflection::to_json("42"), nlohmann::json("42"));
}

TEST(json_helper, to_json_std_string) {
	EXPECT_EQ(putils::reflection::to_json(std::string("42")), nlohmann::json("42"));
}

TEST(json_helper, to_json_array) {
	const int obj[] = { 0, 1, 2 };
	EXPECT_EQ(putils::reflection::to_json(obj), nlohmann::json::parse("[0,1,2]"));
}

TEST(json_helper, to_json_vector) {
	const std::vector<int> obj = { 0, 1, 2 };
	EXPECT_EQ(putils::reflection::to_json(obj), nlohmann::json::parse("[0,1,2]"));
}

TEST(json_helper, to_json_map) {
	const std::map<std::string, int> obj = {
		{ "one", 1 },
		{ "two", 2 }
	};
	EXPECT_EQ(putils::reflection::to_json(obj), nlohmann::json::parse(R"(
{
    "one": 1,
    "two": 2
}
)"));
}

TEST(json_helper, to_json_unordered_map) {
	const std::unordered_map<std::string, int> obj = {
		{ "one", 1 },
		{ "two", 2 }
	};
	EXPECT_EQ(putils::reflection::to_json(obj), nlohmann::json::parse(R"(
{
    "one": 1,
    "two": 2
}
)"));
}

TEST(json_helper, to_json_reflection) {
	EXPECT_EQ(putils::reflection::to_json(reflectible{}), nlohmann::json::parse(R"({"i":42,"s":"hello"})"));
}

TEST(json_helper, from_json_int) {
	int i = 0;
	putils::reflection::from_json(42, i);
	EXPECT_EQ(i, 42);
}

TEST(json_helper, from_json_string) {
	char buff[32];
	putils::reflection::from_json("42", buff);
	EXPECT_STREQ(buff, "42");
}

TEST(json_helper, from_json_std_string) {
	std::string s;
	putils::reflection::from_json("42", s);
	EXPECT_EQ(s, "42");
}

TEST(json_helper, from_json_array) {
	int obj[3] = { -1, -1, -1 };
	putils::reflection::from_json(nlohmann::json::parse("[0,1,2]"), obj);
	for (int i = 0; i < putils::lengthof(obj); ++i)
		EXPECT_EQ(obj[i], i);
}

TEST(json_helper, from_json_vector) {
	std::vector<int> obj;
	putils::reflection::from_json(nlohmann::json::parse("[0,1,2]"), obj);
	EXPECT_EQ(obj.size(), 3);
	for (int i = 0; i < obj.size(); ++i)
		EXPECT_EQ(obj[i], i);
}

TEST(json_helper, from_json_map) {
	std::map<std::string, int> obj;
	putils::reflection::from_json(nlohmann::json::parse(R"(
{
    "one": 1,
    "two": 2
}
)"),
		obj);
	EXPECT_EQ(obj.size(), 2);
	EXPECT_EQ(obj["one"], 1);
	EXPECT_EQ(obj["two"], 2);
}

TEST(json_helper, from_json_unordered_map) {
	std::unordered_map<std::string, int> obj;
	putils::reflection::from_json(nlohmann::json::parse(R"(
{
    "one": 1,
    "two": 2
}
)"),
		obj);
	EXPECT_EQ(obj.size(), 2);
	EXPECT_EQ(obj["one"], 1);
	EXPECT_EQ(obj["two"], 2);
}

TEST(json_helper, from_json_reflection) {
	reflectible obj;
	putils::reflection::from_json(nlohmann::json::parse(R"({"i":-42,"s":"hi"})"), obj);
	EXPECT_EQ(obj.i, -42);
	EXPECT_EQ(obj.s, "hi");
}
