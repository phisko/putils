// gtest
#include <gtest/gtest.h>

// putils
#include "putils/string.hpp"

TEST(string, ctor_cstr) {
	const putils::string<64> ps("hello");
	const std::string ss("hello");
	EXPECT_EQ(ps, ss);
}

TEST(string, ctor_string) {
	const std::string ss("hello");
	const putils::string<64> ps(ss);
	EXPECT_EQ(ps, ss);
}

TEST(string, ctor_string_view) {
	const std::string_view ss("hello");
	const putils::string<64> ps(ss);
	EXPECT_EQ(ps, ss);
}

TEST(string, ctor_format) {
	const std::string_view ss("hello");
	const putils::string<64> ps("{}{}", "hel", "lo");
	EXPECT_EQ(ps, ss);
}

TEST(string, assign_cstr) {
	putils::string<64> ps;
	const std::string ss("hello");
	ps = "hello";
	EXPECT_EQ(ps, ss);
}

TEST(string, assign_string) {
	const std::string ss("hello");
	putils::string<64> ps;
	ps = ss;
	EXPECT_EQ(ps, ss);
}

TEST(string, assign_string_view) {
	const std::string_view ss("hello");
	putils::string<64> ps;
	ps = ss;
	EXPECT_EQ(ps, ss);
}

TEST(string, assign_putils_string) {
	const std::string_view ss("hello");
	putils::string<64> ps;
	ps = putils::string<128>("{}{}", "hel", "lo");
	EXPECT_EQ(ps, ss);
}

TEST(string, assign) {
	putils::string<64> ps;
	ps.assign("hello");
	EXPECT_EQ(ps, "hello");
}

TEST(string, set) {
	putils::string<64> ps;
	ps.set("{}{}", "hel", "lo");
	EXPECT_EQ(ps, "hello");
}

TEST(string, append_cstr) {
	putils::string<64> ps("hel");
	ps += "lo";
	EXPECT_EQ(ps, "hello");
}

TEST(string, append_string_view) {
	putils::string<64> ps("hel");
	ps += std::string_view("lo");
	EXPECT_EQ(ps, "hello");
}

TEST(string, append_putils_string) {
	putils::string<64> ps("hel");
	ps += putils::string<128>("lo");
	EXPECT_EQ(ps, "hello");
}

TEST(string, append_char) {
	putils::string<64> ps("hell");
	ps += 'o';
	EXPECT_EQ(ps, "hello");
}

TEST(string, append_int) {
	putils::string<64> ps("hello");
	ps += -42;
	EXPECT_EQ(ps, "hello-42");
}

TEST(string, append_float) {
	putils::string<64> ps("hello");
	ps += 42.f;
	EXPECT_EQ(ps, "hello42");
}

TEST(string, append__size_t) {
	putils::string<64> ps("hello");
	ps += (size_t)42;
	EXPECT_EQ(ps, "hello42");
}

TEST(string, append_unsigned_int) {
	putils::string<64> ps("hello");
	ps += (unsigned int)42;
	EXPECT_EQ(ps, "hello42");
}

TEST(string, append_int_ptr_t) {
	putils::string<64> ps("hello");
	ps += (intptr_t)42;
	EXPECT_EQ(ps, "hello42");
}

TEST(string, get_appended_cstr) {
	const putils::string<64> ps("hel");
	const putils::string<64> res = ps + "lo";
	EXPECT_EQ(res, "hello");
}

TEST(string, get_appended_string_view) {
	const putils::string<64> ps("hel");
	const putils::string<64> res = ps + std::string_view("lo");
	EXPECT_EQ(res, "hello");
}

TEST(string, get_appended_PutilsString) {
	const putils::string<64> ps("hel");
	const putils::string<64> res = ps + putils::string<128>("lo");
	EXPECT_EQ(res, "hello");
}

TEST(string, get_appended_char) {
	const putils::string<64> ps("hell");
	const putils::string<64> res = ps + 'o';
	EXPECT_EQ(res, "hello");
}

TEST(string, get_appended_int) {
	const putils::string<64> ps("hello");
	const putils::string<64> res = ps + -42;
	EXPECT_EQ(res, "hello-42");
}

TEST(string, get_appended_float) {
	const putils::string<64> ps("hello");
	const putils::string<64> res = ps + 42.f;
	EXPECT_EQ(res, "hello42");
}

TEST(string, get_appended_size_t) {
	const putils::string<64> ps("hello");
	const putils::string<64> res = ps + (size_t)42;
	EXPECT_EQ(res, "hello42");
}

TEST(string, get_appended_unsigned_int) {
	const putils::string<64> ps("hello");
	const putils::string<64> res = ps + (unsigned int)42;
	EXPECT_EQ(res, "hello42");
}

TEST(string, get_appended_int_ptr_t) {
	const putils::string<64> ps("hello");
	const putils::string<64> res = ps + (intptr_t)42;
	EXPECT_EQ(res, "hello42");
}

TEST(string, substr) {
	const putils::string<64> ps("hello");

	putils::string<64> res = ps.substr();
	EXPECT_EQ(res, "hello");

	res = ps.substr(2);
	EXPECT_EQ(res, "llo");

	res = ps.substr(2, 2);
	EXPECT_EQ(res, "ll");
}

TEST(string, clear) {
	putils::string<64> ps("hello");
	ps.clear();
	EXPECT_EQ(ps.size(), 0);
	EXPECT_EQ(ps, "");
}

TEST(string, find_cstr) {
	const putils::string<64> ps("hello");
	EXPECT_EQ(ps.find("hi"), std::string_view::npos);
	EXPECT_EQ(ps.find("hello"), 0);
	EXPECT_EQ(ps.find("llo"), 2);
	EXPECT_EQ(ps.find("hell"), 0);

	EXPECT_EQ(ps.find("hello", 1), std::string_view::npos);
	EXPECT_EQ(ps.find("llo", 1), 2);
}

TEST(string, find_char) {
	const putils::string<64> ps("hello");
	EXPECT_EQ(ps.find('i'), std::string_view::npos);
	EXPECT_EQ(ps.find('h'), 0);
	EXPECT_EQ(ps.find('l'), 2);

	EXPECT_EQ(ps.find('h', 1), std::string_view::npos);
	EXPECT_EQ(ps.find('l', 1), 2);
}

TEST(string, find_string_view) {
	const putils::string<64> ps("hello");
	EXPECT_EQ(ps.find(std::string_view("hi")), std::string_view::npos);
	EXPECT_EQ(ps.find(std::string_view("hello")), 0);
	EXPECT_EQ(ps.find(std::string_view("llo")), 2);
	EXPECT_EQ(ps.find(std::string_view("hell")), 0);

	EXPECT_EQ(ps.find(std::string_view("hello"), 1), std::string_view::npos);
	EXPECT_EQ(ps.find(std::string_view("llo"), 1), 2);
}

TEST(string, at) {
	const putils::string<64> ps("hello");
	EXPECT_EQ(ps.at(0), 'h');
}

TEST(string, str) {
	const putils::string<64> ps("hello");
	EXPECT_EQ(ps.str(), "hello");
}

TEST(string, data) {
	const putils::string<64> ps("hello");
	EXPECT_STREQ(ps.data(), "hello");
}

TEST(string, c_str) {
	const putils::string<64> ps("hello");
	EXPECT_STREQ(ps.c_str(), "hello");
}

TEST(string, size) {
	const putils::string<64> ps("hello");
	EXPECT_EQ(ps.size(), 5);
}

TEST(string, length) {
	const putils::string<64> ps("hello");
	EXPECT_EQ(ps.size(), 5);
}

TEST(string, empty) {
	putils::string<64> ps("hello");
	EXPECT_FALSE(ps.empty());
	ps.clear();
	EXPECT_TRUE(ps.empty());
}

TEST(string, full) {
	putils::string<6> ps("hello");
	EXPECT_TRUE(ps.full());
	ps.clear();
	EXPECT_FALSE(ps.full());
}
