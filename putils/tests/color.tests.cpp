// gtest
#include <gtest/gtest.h>

// putils
#include "putils/color.hpp"

TEST(color, attributes) {
	const putils::color color(1, 2, 3, 4);
	EXPECT_EQ(color.rgba, 0x01020304);
}

TEST(color, to_color) {
	const putils::normalized_color color(1.f, 0.f, 1.f, 1.f);
	const auto converted = putils::to_color(color);
	EXPECT_EQ(converted.r, 255);
	EXPECT_EQ(converted.g, 0);
	EXPECT_EQ(converted.b, 255);
	EXPECT_EQ(converted.a, 255);
}

TEST(color, to_normalized_color) {
	const putils::color color(255, 0, 255, 255);
	const auto converted = putils::to_normalized_color(color);
	EXPECT_EQ(converted.r, 1.f);
	EXPECT_EQ(converted.g, 0.f);
	EXPECT_EQ(converted.b, 1.f);
	EXPECT_EQ(converted.a, 1.f);
}

TEST(color, to_rgba) {
	const putils::normalized_color color(1.f, 0, 1.f, 1.f);
	const auto rgba = putils::to_rgba(color);
	EXPECT_EQ(rgba, 0xff00ffff);
}

TEST(color, from_rgba) {
	const unsigned int rgba = 0xff00ffff;
	const auto color = putils::from_rgba(rgba);
	EXPECT_EQ(color.r, 1.f);
	EXPECT_EQ(color.g, 0.f);
	EXPECT_EQ(color.b, 1.f);
	EXPECT_EQ(color.a, 1.f);
}

TEST(color, to_argb) {
	const putils::color color(1, 2, 3, 4);
	const auto argb = putils::to_argb(color);
	EXPECT_EQ(argb, 0x04010203);
}

TEST(color, toARGB_normalized) {
	const putils::normalized_color color(1.f, 0.f, 1.f, 1.f);
	const auto argb = putils::to_argb(color);
	EXPECT_EQ(argb, 0xffff00ff);
}

TEST(color, from_argb) {
	const unsigned int argb = 0x01020304;
	const auto color = putils::from_argb(argb);
	EXPECT_EQ(color.a, 1);
	EXPECT_EQ(color.r, 2);
	EXPECT_EQ(color.g, 3);
	EXPECT_EQ(color.b, 4);
}

TEST(color, mult) {
	const putils::color a(1, 2, 3, 4);
	const auto res = a * 2.f;
	EXPECT_EQ(res.r, 2);
	EXPECT_EQ(res.g, 4);
	EXPECT_EQ(res.b, 6);
	EXPECT_EQ(res.a, 4);
}

TEST(color, mult_float_first) {
	const putils::color a(1, 2, 3, 4);
	const auto res = 2.f * a;
	EXPECT_EQ(res.r, 2);
	EXPECT_EQ(res.g, 4);
	EXPECT_EQ(res.b, 6);
	EXPECT_EQ(res.a, 4);
}

TEST(color, mult_assign) {
	putils::color a(1, 2, 3, 4);
	a *= 2.f;
	EXPECT_EQ(a.r, 2);
	EXPECT_EQ(a.g, 4);
	EXPECT_EQ(a.b, 6);
	EXPECT_EQ(a.a, 4);
}

TEST(color, div) {
	const putils::color a(2, 4, 6, 8);
	const auto res = a / 2.f;
	EXPECT_EQ(res.r, 1);
	EXPECT_EQ(res.g, 2);
	EXPECT_EQ(res.b, 3);
	EXPECT_EQ(res.a, 8);
}

TEST(color, div_assign) {
	putils::color a(2, 4, 6, 8);
	a /= 2.f;
	EXPECT_EQ(a.r, 1);
	EXPECT_EQ(a.g, 2);
	EXPECT_EQ(a.b, 3);
	EXPECT_EQ(a.a, 8);
}

TEST(color, mult_normalized) {
	const putils::normalized_color a(.1f, .2f, .3f, .4f);
	const auto res = a * 2.f;
	EXPECT_FLOAT_EQ(res.r, .2f);
	EXPECT_FLOAT_EQ(res.g, .4f);
	EXPECT_FLOAT_EQ(res.b, .6f);
	EXPECT_FLOAT_EQ(res.a, .4f);
}

TEST(color, mult_float_first_normalized) {
	const putils::normalized_color a(.1f, .2f, .3f, .4f);
	const auto res = 2.f * a;
	EXPECT_FLOAT_EQ(res.r, .2f);
	EXPECT_FLOAT_EQ(res.g, .4f);
	EXPECT_FLOAT_EQ(res.b, .6f);
	EXPECT_FLOAT_EQ(res.a, .4f);
}

TEST(color, mult_assign_normalized) {
	putils::normalized_color a(.1f, .2f, .3f, .4f);
	a *= 2.f;
	EXPECT_FLOAT_EQ(a.r, .2f);
	EXPECT_FLOAT_EQ(a.g, .4f);
	EXPECT_FLOAT_EQ(a.b, .6f);
	EXPECT_FLOAT_EQ(a.a, .4f);
}

TEST(color, div_normalized) {
	const putils::normalized_color a(.2f, .4f, .6f, .8f);
	const auto res = a / 2.f;
	EXPECT_FLOAT_EQ(res.r, .1f);
	EXPECT_FLOAT_EQ(res.g, .2f);
	EXPECT_FLOAT_EQ(res.b, .3f);
	EXPECT_FLOAT_EQ(res.a, .8f);
}

TEST(color, div_assign_normalized) {
	putils::normalized_color a(.2f, .4f, .6f, .8f);
	a /= 2.f;
	EXPECT_FLOAT_EQ(a.r, .1f);
	EXPECT_FLOAT_EQ(a.g, .2f);
	EXPECT_FLOAT_EQ(a.b, .3f);
	EXPECT_FLOAT_EQ(a.a, .8f);
}

TEST(color, mult_normalized_normalized) {
	const putils::normalized_color a(.1f, .2f, .3f, .4f);
	const putils::normalized_color b(.2f, .3f, .4f, .5f);
	const auto res = a * b;
	EXPECT_FLOAT_EQ(res.r, .02f);
	EXPECT_FLOAT_EQ(res.g, .06f);
	EXPECT_FLOAT_EQ(res.b, .12f);
	EXPECT_FLOAT_EQ(res.a, .20f);
}

TEST(color, mult_assign_normalized_normalized) {
	putils::normalized_color a(.1f, .2f, .3f, .4f);
	const putils::normalized_color b(.2f, .3f, .4f, .5f);
	a *= b;
	EXPECT_FLOAT_EQ(a.r, .02f);
	EXPECT_FLOAT_EQ(a.g, .06f);
	EXPECT_FLOAT_EQ(a.b, .12f);
	EXPECT_FLOAT_EQ(a.a, .20f);
}

TEST(color, div_normalized_normalized) {
	const putils::normalized_color a(.2f, .06f, .12f, .20f);
	const putils::normalized_color b(.2f, .3f, .4f, .5f);
	const auto res = a / b;
	EXPECT_FLOAT_EQ(res.r, 1.f);
	EXPECT_FLOAT_EQ(res.g, .2f);
	EXPECT_FLOAT_EQ(res.b, .3f);
	EXPECT_FLOAT_EQ(res.a, .4f);
}

TEST(color, div_assign_normalized_normalized) {
	putils::normalized_color a(.2f, .06f, .12f, .20f);
	const putils::normalized_color b(.2f, .3f, .4f, .5f);
	a /= b;
	EXPECT_FLOAT_EQ(a.r, 1.f);
	EXPECT_FLOAT_EQ(a.g, .2f);
	EXPECT_FLOAT_EQ(a.b, .3f);
	EXPECT_FLOAT_EQ(a.a, .4f);
}