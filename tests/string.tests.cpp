#include <gtest/gtest.h>
#include "string.hpp"

TEST(string, ctorCstr) {
    const putils::string<64> ps("hello");
    const std::string ss("hello");
    EXPECT_EQ(ps, ss);
}

TEST(string, ctorString) {
    const std::string ss("hello");
    const putils::string<64> ps(ss);
    EXPECT_EQ(ps, ss);
}

TEST(string, ctorStringView) {
    const std::string_view ss("hello");
    const putils::string<64> ps(ss);
    EXPECT_EQ(ps, ss);
}

TEST(string, ctorFormat) {
    const std::string_view ss("hello");
    const putils::string<64> ps("%s%s", "hel", "lo");
    EXPECT_EQ(ps, ss);
}

TEST(string, assignCstr) {
    putils::string<64> ps;
    const std::string ss("hello");
    ps = "hello";
    EXPECT_EQ(ps, ss);
}

TEST(string, assignString) {
    const std::string ss("hello");
    putils::string<64> ps;
    ps = ss;
    EXPECT_EQ(ps, ss);
}

TEST(string, assignStringView) {
    const std::string_view ss("hello");
    putils::string<64> ps;
    ps = ss;
    EXPECT_EQ(ps, ss);
}

TEST(string, assignPutilsString) {
    const std::string_view ss("hello");
    putils::string<64> ps;
    ps = putils::string<128>("%s%s", "hel", "lo");
    EXPECT_EQ(ps, ss);
}

TEST(string, assign) {
    putils::string<64> ps;
    ps.assign("hello");
    EXPECT_EQ(ps, "hello");
}

TEST(string, set) {
    putils::string<64> ps;
    ps.set("%s%s", "hel", "lo");
    EXPECT_EQ(ps, "hello");
}

TEST(string, appendCStr) {
    putils::string<64> ps("hel");
    ps += "lo";
    EXPECT_EQ(ps, "hello");
}

TEST(string, appendStringView) {
    putils::string<64> ps("hel");
    ps += std::string_view("lo");
    EXPECT_EQ(ps, "hello");
}

TEST(string, appendPutilsString) {
    putils::string<64> ps("hel");
    ps += putils::string<128>("lo");
    EXPECT_EQ(ps, "hello");
}

TEST(string, appendChar) {
    putils::string<64> ps("hell");
    ps += 'o';
    EXPECT_EQ(ps, "hello");
}

TEST(string, appendInt) {
    putils::string<64> ps("hello");
    ps += -42;
    EXPECT_EQ(ps, "hello-42");
}

TEST(string, appendFloat) {
    putils::string<64> ps("hello");
    ps += 42.f;
    EXPECT_EQ(ps, "hello42.000000");
}

TEST(string, appendSize_t) {
    putils::string<64> ps("hello");
    ps += (size_t)42;
    EXPECT_EQ(ps, "hello42");
}

TEST(string, appendUnsignedInt) {
    putils::string<64> ps("hello");
    ps += (unsigned int)42;
    EXPECT_EQ(ps, "hello42");
}

TEST(string, appendInt_ptr_t) {
    putils::string<64> ps("hello");
    ps += (intptr_t)42;
    EXPECT_EQ(ps, "hello42");
}

TEST(string, getAppendedCStr) {
    const putils::string<64> ps("hel");
    const putils::string<64> res = ps + "lo";
    EXPECT_EQ(res, "hello");
}

TEST(string, getAppendedStringView) {
    const putils::string<64> ps("hel");
    const putils::string<64> res = ps + std::string_view("lo");
    EXPECT_EQ(res, "hello");
}

TEST(string, getAppendedPutilsString) {
    const putils::string<64> ps("hel");
    const putils::string<64> res = ps + putils::string<128>("lo");
    EXPECT_EQ(res, "hello");
}

TEST(string, getAppendedChar) {
    const putils::string<64> ps("hell");
    const putils::string<64> res = ps + 'o';
    EXPECT_EQ(res, "hello");
}

TEST(string, getAppendedInt) {
    const putils::string<64> ps("hello");
    const putils::string<64> res = ps + -42;
    EXPECT_EQ(res, "hello-42");
}

TEST(string, getAppendedFloat) {
    const putils::string<64> ps("hello");
    const putils::string<64> res = ps + 42.f;
    EXPECT_EQ(res, "hello42.000000");
}

TEST(string, getAppendedSize_t) {
    const putils::string<64> ps("hello");
    const putils::string<64> res = ps + (size_t)42;
    EXPECT_EQ(res, "hello42");
}

TEST(string, getAppendedUnsignedInt) {
    const putils::string<64> ps("hello");
    const putils::string<64> res = ps + (unsigned int)42;
    EXPECT_EQ(res, "hello42");
}

TEST(string, getAppendedInt_ptr_t) {
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

TEST(string, findCStr) {
    const putils::string<64> ps("hello");
    EXPECT_EQ(ps.find("hi"), std::string_view::npos);
    EXPECT_EQ(ps.find("hello"), 0);
    EXPECT_EQ(ps.find("llo"), 2);
    EXPECT_EQ(ps.find("hell"), 0);

    EXPECT_EQ(ps.find("hello", 1), std::string_view::npos);
    EXPECT_EQ(ps.find("llo", 1), 2);
}

TEST(string, findChar) {
    const putils::string<64> ps("hello");
    EXPECT_EQ(ps.find('i'), std::string_view::npos);
    EXPECT_EQ(ps.find('h'), 0);
    EXPECT_EQ(ps.find('l'), 2);

    EXPECT_EQ(ps.find('h', 1), std::string_view::npos);
    EXPECT_EQ(ps.find('l', 1), 2);
}

TEST(string, findStringView) {
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
