// gtest
#include <gtest/gtest.h>

// putils
#include "putils/point.hpp"

TEST(point2, construct_other_type) {
	const putils::point2f p(42.5f, -42.f);
	const putils::point2i p2(p);

	EXPECT_EQ(p2.x, 42);
	EXPECT_EQ(p2.y, -42);
}

TEST(point2, construct_point3) {
	const putils::point3f p(42.5f, -42.f, 100.f);
	const putils::point2i p2(p);

	EXPECT_EQ(p2.x, 42);
	EXPECT_EQ(p2.y, -42);
}

TEST(point2, index_operator) {
	const putils::point2i p{ 42, 84 };
	EXPECT_EQ(p[0], 42);
	EXPECT_EQ(p[1], 84);
}

TEST(point3, construct_other_type) {
	const putils::point3f p(42.5f, -42.f, 84.f);
	const putils::point3i p2(p);

	EXPECT_EQ(p2.x, 42);
	EXPECT_EQ(p2.y, -42);
	EXPECT_EQ(p2.z, 84);
}

TEST(point3, construct_point2) {
	const putils::point2f p(42.5f, -42.f);
	const putils::point3i p2(p);

	EXPECT_EQ(p2.x, 42);
	EXPECT_EQ(p2.y, -42);
	EXPECT_EQ(p2.z, 0);
}

TEST(point3, index_operator) {
	const putils::point3i p{ 42, 84, -42 };
	EXPECT_EQ(p[0], 42);
	EXPECT_EQ(p[1], 84);
	EXPECT_EQ(p[2], -42);
}

TEST(point2, comparison) {
	const putils::point2i p{ 42, 84 };
	const putils::point2f p2{ 42.f, 84.f };
	const putils::point2f p3{ 0.f, 0.f };
	EXPECT_EQ(p, p2);
	EXPECT_NE(p, p3);
}

TEST(point3, comparison) {
	const putils::point3i p{ 42, 84, -42 };
	const putils::point3f p2{ 42.f, 84.f, -42.f };
	const putils::point3f p3{ 0.f, 0.f, 0.f };
	EXPECT_EQ(p, p2);
	EXPECT_NE(p, p3);
}

TEST(point2, addition) {
	const putils::point2i p{ 42, 84 };
	const putils::point2f p2{ 42.f, 84.f };
	const putils::point2i p3 = p + p2;
	EXPECT_EQ(p3.x, 84);
	EXPECT_EQ(p3.y, 168);

	putils::point2i p4 = p;
	p4 += p2;
	EXPECT_EQ(p3, p4);
}

TEST(point3, addition) {
	const putils::point3i p{ 42, 84, -42 };
	const putils::point3f p2{ 42.f, 84.f, -42.f };
	const putils::point3i p3 = p + p2;
	EXPECT_EQ(p3.x, 84);
	EXPECT_EQ(p3.y, 168);
	EXPECT_EQ(p3.z, -84);

	putils::point3i p4 = p;
	p4 += p2;
	EXPECT_EQ(p3, p4);
}

TEST(point2, substraction) {
	const putils::point2i p{ 42, 84 };
	const putils::point2f p2{ 42.f, 84.f };
	const putils::point2i p3 = p - p2;
	EXPECT_EQ(p3.x, 0);
	EXPECT_EQ(p3.y, 0);

	putils::point2i p4 = p;
	p4 -= p2;
	EXPECT_EQ(p3, p4);
}

TEST(point3, substraction) {
	const putils::point3i p{ 42, 84, -42 };
	const putils::point3f p2{ 42.f, 84.f, -42.f };
	const putils::point3i p3 = p - p2;
	EXPECT_EQ(p3.x, 0);
	EXPECT_EQ(p3.y, 0);
	EXPECT_EQ(p3.z, 0);

	putils::point3i p4 = p;
	p4 -= p2;
	EXPECT_EQ(p3, p4);
}

TEST(point2, multiplication) {
	const putils::point2i p{ 1, 2 };
	const putils::point2f p2{ 3, 4 };
	const putils::point2i p3 = p * p2;
	EXPECT_EQ(p3.x, 3);
	EXPECT_EQ(p3.y, 8);

	putils::point2i p4 = p;
	p4 *= p2;
	EXPECT_EQ(p3, p4);
}

TEST(point3, multiplication) {
	const putils::point3i p{ 1, 2, 3 };
	const putils::point3f p2{ 4, 5, 6 };
	const putils::point3i p3 = p * p2;
	EXPECT_EQ(p3.x, 4);
	EXPECT_EQ(p3.y, 10);
	EXPECT_EQ(p3.z, 18);

	putils::point3i p4 = p;
	p4 *= p2;
	EXPECT_TRUE(p3 == p4);
}

TEST(point2, division) {
	const putils::point2f p{ 3, 4 };
	const putils::point2i p2{ 1, 2 };
	const putils::point2i p3 = p / p2;
	EXPECT_EQ(p3.x, 3);
	EXPECT_EQ(p3.y, 2);

	putils::point2i p4 = p;
	p4 /= p2;
	EXPECT_TRUE(p3 == p4);
}

TEST(point3, division) {
	const putils::point3f p{ 4, 5, 6 };
	const putils::point3i p2{ 1, 2, 3 };
	const putils::point3i p3 = p / p2;
	EXPECT_EQ(p3.x, 4);
	EXPECT_EQ(p3.y, 2);
	EXPECT_EQ(p3.z, 2);

	putils::point3i p4 = p;
	p4 /= p2;
	EXPECT_TRUE(p3 == p4);
}

TEST(point2, dot) {
	const putils::point2i p{ 1, 2 };
	const auto dot = putils::dot(p, p);
	EXPECT_EQ(dot, 5);
}

TEST(point3, dot) {
	const putils::point3i p{ 1, 2, 3 };
	const auto dot = putils::dot(p, p);
	EXPECT_EQ(dot, 14);
}

TEST(point2, negative) {
	const putils::point2i p{ 1, 2 };
	const putils::point2i p2 = -p;

	EXPECT_EQ(p2.x, -1);
	EXPECT_EQ(p2.y, -2);
}

TEST(point3, negative) {
	const putils::point3i p{ 1, 2, 3 };
	const putils::point3i p2 = -p;

	EXPECT_EQ(p2.x, -1);
	EXPECT_EQ(p2.y, -2);
	EXPECT_EQ(p2.z, -3);
}

TEST(point2, multiplication_float) {
	const putils::point2i p{ 1, 2 };
	const putils::point2i p2 = p * 2.f;

	EXPECT_EQ(p2.x, 2);
	EXPECT_EQ(p2.y, 4);

	putils::point2i p3 = p;
	p3 *= 2.f;
	EXPECT_TRUE(p2 == p3);
}

TEST(point3, multiplication_float) {
	const putils::point3i p{ 1, 2, 3 };
	const putils::point3i p2 = p * 2.f;

	EXPECT_EQ(p2.x, 2);
	EXPECT_EQ(p2.y, 4);
	EXPECT_EQ(p2.z, 6);

	putils::point3i p3 = p;
	p3 *= 2.f;
	EXPECT_TRUE(p2 == p3);
}

TEST(point2, division_float) {
	const putils::point2i p{ 2, 4 };
	const putils::point2i p2 = p / 2.f;

	EXPECT_EQ(p2.x, 1);
	EXPECT_EQ(p2.y, 2);

	putils::point2i p3 = p;
	p3 /= 2.f;
	EXPECT_TRUE(p2 == p3);
}

TEST(point3, division_float) {
	const putils::point3i p{ 2, 4, 6 };
	const putils::point3i p2 = p / 2.f;

	EXPECT_EQ(p2.x, 1);
	EXPECT_EQ(p2.y, 2);
	EXPECT_EQ(p2.z, 3);

	putils::point3i p3 = p;
	p3 /= 2.f;
	EXPECT_TRUE(p2 == p3);
}

TEST(point2, get_length_squared) {
	const putils::point2i p{ 1, 0 };
	EXPECT_EQ(putils::get_length_squared(p), 1);

	const putils::point2i p2{ 2, 0 };
	EXPECT_EQ(putils::get_length_squared(p2), 4);

	const putils::point2i p3{ 1, 1 };
	EXPECT_EQ(putils::get_length_squared(p3), 2);
}

TEST(point2, get_length) {
	const putils::point2i p{ 1, 0 };
	EXPECT_EQ(putils::get_length(p), 1);

	const putils::point2i p2{ 2, 0 };
	EXPECT_EQ(putils::get_length(p2), 2);

	const putils::point2i p3{ 1, 1 };
	EXPECT_FLOAT_EQ(putils::get_length(p3), sqrtf(2.f));
}

TEST(point2, normalize) {
	putils::point2f p{ 1, 0 };
	putils::normalize(p);
	EXPECT_EQ(p.x, 1.f);
	EXPECT_EQ(p.y, 0.f);

	p = { 100.f, 100.f };
	putils::normalize(p);
	EXPECT_FLOAT_EQ(p.x, .707107f);
	EXPECT_FLOAT_EQ(p.y, .707107f);

	p = { 1, 0 };
	putils::normalize(p, 2.f);
	EXPECT_EQ(p.x, 2.f);
	EXPECT_EQ(p.y, 0.f);
}

TEST(point3, normalize) {
	putils::point3f p{ 1, 0, 0 };
	putils::normalize(p);
	EXPECT_EQ(p.x, 1.f);
	EXPECT_EQ(p.y, 0.f);
	EXPECT_EQ(p.z, 0.f);

	p = { 100.f, 100.f, 100.f };
	putils::normalize(p);
	EXPECT_FLOAT_EQ(p.x, .57735026f);
	EXPECT_FLOAT_EQ(p.y, .57735026f);
	EXPECT_FLOAT_EQ(p.z, .57735026f);

	p = { 1, 0, 0 };
	putils::normalize(p, 2.f);
	EXPECT_EQ(p.x, 2.f);
	EXPECT_EQ(p.y, 0.f);
	EXPECT_EQ(p.z, 0.f);
}

TEST(point2, normalized) {
	putils::point2f p{ 1, 0 };
	const putils::point2f p2 = putils::normalized(p);
	putils::normalize(p);
	EXPECT_TRUE(p == p2);

	p = { 100.f, 100.f };
	const putils::point2f p3 = putils::normalized(p);
	putils::normalize(p);
	EXPECT_TRUE(p == p3);
}

TEST(point3, normalized) {
	putils::point3f p{ 1, 0, 0 };
	const putils::point3f p2 = putils::normalized(p);
	putils::normalize(p);
	EXPECT_TRUE(p == p2);

	p = { 100.f, 100.f, 100.f };
	const putils::point3f p3 = putils::normalized(p);
	putils::normalize(p);
	EXPECT_TRUE(p == p3);
}
