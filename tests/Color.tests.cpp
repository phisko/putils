// gtest
#include <gtest/gtest.h>

// putils
#include "Color.hpp"

TEST(Color, attributes) {
    const putils::Color color(1, 2, 3, 4);
    EXPECT_EQ(color.rgba, 0x01020304);
}

TEST(Color, toColor) {
    const putils::NormalizedColor color(1.f, 0.f, 1.f, 1.f);
    const auto converted = putils::toColor(color);
    EXPECT_EQ(converted.r, 255);
    EXPECT_EQ(converted.g, 0);
    EXPECT_EQ(converted.b, 255);
    EXPECT_EQ(converted.a, 255);
}

TEST(Color, toNormalizedColor) {
    const putils::Color color(255, 0, 255, 255);
    const auto converted = putils::toNormalizedColor(color);
    EXPECT_EQ(converted.r, 1.f);
    EXPECT_EQ(converted.g, 0.f);
    EXPECT_EQ(converted.b, 1.f);
    EXPECT_EQ(converted.a, 1.f);
}

TEST(Color, toRGBA) {
    const putils::NormalizedColor color(1.f, 0, 1.f, 1.f);
    const auto rgba = putils::toRGBA(color);
    EXPECT_EQ(rgba, 0xff00ffff);
}

TEST(Color, fromRGBA) {
    const unsigned int rgba = 0xff00ffff;
    const auto color = putils::fromRGBA(rgba);
    EXPECT_EQ(color.r, 1.f);
    EXPECT_EQ(color.g, 0.f);
    EXPECT_EQ(color.b, 1.f);
    EXPECT_EQ(color.a, 1.f);
}

TEST(Color, toARGB) {
    const putils::Color color(1, 2, 3, 4);
    const auto argb = putils::toARGB(color);
    EXPECT_EQ(argb, 0x04010203);
}

TEST(Color, toARGB_Normalized) {
    const putils::NormalizedColor color(1.f, 0.f, 1.f, 1.f);
    const auto argb = putils::toARGB(color);
    EXPECT_EQ(argb, 0xffff00ff);
}

TEST(Color, fromARGB) {
    const unsigned int argb = 0x01020304;
    const auto color = putils::fromARGB(argb);
    EXPECT_EQ(color.a, 1);
    EXPECT_EQ(color.r, 2);
    EXPECT_EQ(color.g, 3);
    EXPECT_EQ(color.b, 4);
}

TEST(Color, mult) {
    const putils::Color a(1, 2, 3, 4);
    const auto res = a * 2.f;
    EXPECT_EQ(res.r, 2);
    EXPECT_EQ(res.g, 4);
    EXPECT_EQ(res.b, 6);
    EXPECT_EQ(res.a, 4);
}

TEST(Color, mult_float_first) {
    const putils::Color a(1, 2, 3, 4);
    const auto res = 2.f * a;
    EXPECT_EQ(res.r, 2);
    EXPECT_EQ(res.g, 4);
    EXPECT_EQ(res.b, 6);
    EXPECT_EQ(res.a, 4);
}

TEST(Color, mult_assign) {
    putils::Color a(1, 2, 3, 4);
    a *= 2.f;
    EXPECT_EQ(a.r, 2);
    EXPECT_EQ(a.g, 4);
    EXPECT_EQ(a.b, 6);
    EXPECT_EQ(a.a, 4);
}

TEST(Color, div) {
    const putils::Color a(2, 4, 6, 8);
    const auto res = a / 2.f;
    EXPECT_EQ(res.r, 1);
    EXPECT_EQ(res.g, 2);
    EXPECT_EQ(res.b, 3);
    EXPECT_EQ(res.a, 8);
}

TEST(Color, div_assign) {
    putils::Color a(2, 4, 6, 8);
    a /= 2.f;
    EXPECT_EQ(a.r, 1);
    EXPECT_EQ(a.g, 2);
    EXPECT_EQ(a.b, 3);
    EXPECT_EQ(a.a, 8);
}

TEST(Color, mult_Normalized) {
    const putils::NormalizedColor a(.1f, .2f, .3f, .4f);
    const auto res = a * 2.f;
    EXPECT_FLOAT_EQ(res.r, .2f);
    EXPECT_FLOAT_EQ(res.g, .4f);
    EXPECT_FLOAT_EQ(res.b, .6f);
    EXPECT_FLOAT_EQ(res.a, .4f);
}

TEST(Color, mult_float_first_Normalized) {
    const putils::NormalizedColor a(.1f, .2f, .3f, .4f);
    const auto res = 2.f * a;
    EXPECT_FLOAT_EQ(res.r, .2f);
    EXPECT_FLOAT_EQ(res.g, .4f);
    EXPECT_FLOAT_EQ(res.b, .6f);
    EXPECT_FLOAT_EQ(res.a, .4f);
}

TEST(Color, mult_assign_Normalized) {
    putils::NormalizedColor a(.1f, .2f, .3f, .4f);
    a *= 2.f;
    EXPECT_FLOAT_EQ(a.r, .2f);
    EXPECT_FLOAT_EQ(a.g, .4f);
    EXPECT_FLOAT_EQ(a.b, .6f);
    EXPECT_FLOAT_EQ(a.a, .4f);
}

TEST(Color, div_Normalized) {
    const putils::NormalizedColor a(.2f, .4f, .6f, .8f);
    const auto res = a / 2.f;
    EXPECT_FLOAT_EQ(res.r, .1f);
    EXPECT_FLOAT_EQ(res.g, .2f);
    EXPECT_FLOAT_EQ(res.b, .3f);
    EXPECT_FLOAT_EQ(res.a, .8f);
}

TEST(Color, div_assign_Normalized) {
    putils::NormalizedColor a(.2f, .4f, .6f, .8f);
    a /= 2.f;
    EXPECT_FLOAT_EQ(a.r, .1f);
    EXPECT_FLOAT_EQ(a.g, .2f);
    EXPECT_FLOAT_EQ(a.b, .3f);
    EXPECT_FLOAT_EQ(a.a, .8f);
}

TEST(Color, mult_NormalizedNormalized) {
    const putils::NormalizedColor a(.1f, .2f, .3f, .4f);
    const putils::NormalizedColor b(.2f, .3f, .4f, .5f);
    const auto res = a * b;
    EXPECT_FLOAT_EQ(res.r, .02f);
    EXPECT_FLOAT_EQ(res.g, .06f);
    EXPECT_FLOAT_EQ(res.b, .12f);
    EXPECT_FLOAT_EQ(res.a, .20f);
}

TEST(Color, mult_assign_NormalizedNormalized) {
    putils::NormalizedColor a(.1f, .2f, .3f, .4f);
    const putils::NormalizedColor b(.2f, .3f, .4f, .5f);
    a *= b;
    EXPECT_FLOAT_EQ(a.r, .02f);
    EXPECT_FLOAT_EQ(a.g, .06f);
    EXPECT_FLOAT_EQ(a.b, .12f);
    EXPECT_FLOAT_EQ(a.a, .20f);
}

TEST(Color, div_NormalizedNormalized) {
    const putils::NormalizedColor a(.2f, .06f, .12f, .20f);
    const putils::NormalizedColor b(.2f, .3f, .4f, .5f);
    const auto res = a / b;
    EXPECT_FLOAT_EQ(res.r, 1.f);
    EXPECT_FLOAT_EQ(res.g, .2f);
    EXPECT_FLOAT_EQ(res.b, .3f);
    EXPECT_FLOAT_EQ(res.a, .4f);
}

TEST(Color, div_assign_NormalizedNormalized) {
    putils::NormalizedColor a(.2f, .06f, .12f, .20f);
    const putils::NormalizedColor b(.2f, .3f, .4f, .5f);
    a /= b;
    EXPECT_FLOAT_EQ(a.r, 1.f);
    EXPECT_FLOAT_EQ(a.g, .2f);
    EXPECT_FLOAT_EQ(a.b, .3f);
    EXPECT_FLOAT_EQ(a.a, .4f);
}