#include <gtest/gtest.h>
#include "Point.hpp"

TEST(Point2, constructOtherType) {
    const putils::Point2f p(42.5f, -42.f);
    const putils::Point2i p2(p);

    EXPECT_EQ(p2.x, 42);
    EXPECT_EQ(p2.y, -42);
}

TEST(Point2, constructPoint3) {
    const putils::Point3f p(42.5f, -42.f, 100.f);
    const putils::Point2i p2(p);

    EXPECT_EQ(p2.x, 42);
    EXPECT_EQ(p2.y, -42);
}

TEST(Point2, indexOperator) {
    const putils::Point2i p{ 42, 84 };
    EXPECT_EQ(p[0], 42);
    EXPECT_EQ(p[1], 84);
}

TEST(Point3, constructOtherType) {
    const putils::Point3f p(42.5f, -42.f, 84.f);
    const putils::Point3i p2(p);

    EXPECT_EQ(p2.x, 42);
    EXPECT_EQ(p2.y, -42);
    EXPECT_EQ(p2.z, 84);
}

TEST(Point3, constructPoint2) {
    const putils::Point2f p(42.5f, -42.f);
    const putils::Point3i p2(p);

    EXPECT_EQ(p2.x, 42);
    EXPECT_EQ(p2.y, -42);
    EXPECT_EQ(p2.z, 0);
}

TEST(Point3, indexOperator) {
    const putils::Point3i p{ 42, 84, -42 };
    EXPECT_EQ(p[0], 42);
    EXPECT_EQ(p[1], 84);
    EXPECT_EQ(p[2], -42);
}

TEST(Point2, comparison) {
    const putils::Point2i p{ 42, 84 };
    const putils::Point2f p2{ 42.f, 84.f };
    const putils::Point2f p3{ 0.f, 0.f };
    EXPECT_EQ(p, p2);
    EXPECT_NE(p, p3);
}

TEST(Point3, comparison) {
    const putils::Point3i p{ 42, 84, -42 };
    const putils::Point3f p2{ 42.f, 84.f, -42.f };
    const putils::Point3f p3{ 0.f, 0.f, 0.f };
    EXPECT_EQ(p, p2);
    EXPECT_NE(p, p3);
}

TEST(Point2, addition) {
    const putils::Point2i p{ 42, 84 };
    const putils::Point2f p2{ 42.f, 84.f };
    const putils::Point2i p3 = p + p2;
    EXPECT_EQ(p3.x, 84);
    EXPECT_EQ(p3.y, 168);

    putils::Point2i p4 = p;
    p4 += p2;
    EXPECT_EQ(p3, p4);
}

TEST(Point3, addition) {
    const putils::Point3i p{ 42, 84, -42 };
    const putils::Point3f p2{ 42.f, 84.f, -42.f };
    const putils::Point3i p3 = p + p2;
    EXPECT_EQ(p3.x, 84);
    EXPECT_EQ(p3.y, 168);
    EXPECT_EQ(p3.z, -84);

    putils::Point3i p4 = p;
    p4 += p2;
    EXPECT_EQ(p3, p4);
}

TEST(Point2, substraction) {
    const putils::Point2i p{ 42, 84 };
    const putils::Point2f p2{ 42.f, 84.f };
    const putils::Point2i p3 = p - p2;
    EXPECT_EQ(p3.x, 0);
    EXPECT_EQ(p3.y, 0);

    putils::Point2i p4 = p;
    p4 -= p2;
    EXPECT_EQ(p3, p4);
}

TEST(Point3, substraction) {
    const putils::Point3i p{ 42, 84, -42 };
    const putils::Point3f p2{ 42.f, 84.f, -42.f };
    const putils::Point3i p3 = p - p2;
    EXPECT_EQ(p3.x, 0);
    EXPECT_EQ(p3.y, 0);
    EXPECT_EQ(p3.z, 0);

    putils::Point3i p4 = p;
    p4 -= p2;
    EXPECT_EQ(p3, p4);
}

TEST(Point2, multiplication) {
    const putils::Point2i p{ 1, 2 };
    const putils::Point2f p2{ 3, 4 };
    const putils::Point2i p3 = p * p2;
    EXPECT_EQ(p3.x, 3);
    EXPECT_EQ(p3.y, 8);

    putils::Point2i p4 = p;
    p4 *= p2;
    EXPECT_EQ(p3, p4);
}

TEST(Point3, multiplication) {
    const putils::Point3i p{ 1, 2, 3 };
    const putils::Point3f p2{ 4, 5, 6 };
    const putils::Point3i p3 = p * p2;
    EXPECT_EQ(p3.x, 4);
    EXPECT_EQ(p3.y, 10);
    EXPECT_EQ(p3.z, 18);

    putils::Point3i p4 = p;
    p4 *= p2;
    EXPECT_TRUE(p3 == p4);
}

TEST(Point2, division) {
    const putils::Point2f p{ 3, 4 };
    const putils::Point2i p2{ 1, 2 };
    const putils::Point2i p3 = p / p2;
    EXPECT_EQ(p3.x, 3);
    EXPECT_EQ(p3.y, 2);

    putils::Point2i p4 = p;
    p4 /= p2;
    EXPECT_TRUE(p3 == p4);
}

TEST(Point3, division) {
    const putils::Point3f p{ 4, 5, 6 };
    const putils::Point3i p2{ 1, 2, 3 };
    const putils::Point3i p3 = p / p2;
    EXPECT_EQ(p3.x, 4);
    EXPECT_EQ(p3.y, 2);
    EXPECT_EQ(p3.z, 2);

    putils::Point3i p4 = p;
    p4 /= p2;
    EXPECT_TRUE(p3 == p4);
}

TEST(Point2, dot) {
    const putils::Point2i p{ 1, 2 };
    const auto dot = putils::dot(p, p);
    EXPECT_EQ(dot, 5);
}

TEST(Point3, dot) {
    const putils::Point3i p{ 1, 2, 3 };
    const auto dot = putils::dot(p, p);
    EXPECT_EQ(dot, 14);
}

TEST(Point2, negative) {
    const putils::Point2i p{ 1, 2 };
    const putils::Point2i p2 = -p;

    EXPECT_EQ(p2.x, -1);
    EXPECT_EQ(p2.y, -2);
}

TEST(Point3, negative) {
    const putils::Point3i p{ 1, 2, 3 };
    const putils::Point3i p2 = -p;

    EXPECT_EQ(p2.x, -1);
    EXPECT_EQ(p2.y, -2);
    EXPECT_EQ(p2.z, -3);
}

TEST(Point2, multiplicationFloat) {
    const putils::Point2i p{ 1, 2 };
    const putils::Point2i p2 = p * 2.f;

    EXPECT_EQ(p2.x, 2);
    EXPECT_EQ(p2.y, 4);

    putils::Point2i p3 = p;
    p3 *= 2.f;
    EXPECT_TRUE(p2 == p3);
}

TEST(Point3, multiplicationFloat) {
    const putils::Point3i p{ 1, 2, 3 };
    const putils::Point3i p2 = p * 2.f;

    EXPECT_EQ(p2.x, 2);
    EXPECT_EQ(p2.y, 4);
    EXPECT_EQ(p2.z, 6);

    putils::Point3i p3 = p;
    p3 *= 2.f;
    EXPECT_TRUE(p2 == p3);
}

TEST(Point2, divisionFloat) {
    const putils::Point2i p{ 2, 4 };
    const putils::Point2i p2 = p / 2.f;

    EXPECT_EQ(p2.x, 1);
    EXPECT_EQ(p2.y, 2);

    putils::Point2i p3 = p;
    p3 /= 2.f;
    EXPECT_TRUE(p2 == p3);
}

TEST(Point3, divisionFloat) {
    const putils::Point3i p{ 2, 4, 6 };
    const putils::Point3i p2 = p / 2.f;

    EXPECT_EQ(p2.x, 1);
    EXPECT_EQ(p2.y, 2);
    EXPECT_EQ(p2.z, 3);

    putils::Point3i p3 = p;
    p3 /= 2.f;
    EXPECT_TRUE(p2 == p3);
}

TEST(Point2, getLengthSquared) {
    const putils::Point2i p{ 1, 0 };
    EXPECT_EQ(putils::getLengthSquared(p), 1);

    const putils::Point2i p2{ 2, 0 };
    EXPECT_EQ(putils::getLengthSquared(p2), 4);

    const putils::Point2i p3{ 1, 1 };
    EXPECT_EQ(putils::getLengthSquared(p3), 2);
}

TEST(Point2, getLength) {
    const putils::Point2i p{ 1, 0 };
    EXPECT_EQ(putils::getLength(p), 1);

    const putils::Point2i p2{ 2, 0 };
    EXPECT_EQ(putils::getLength(p2), 2);

    const putils::Point2i p3{ 1, 1 };
    EXPECT_FLOAT_EQ(putils::getLength(p3), sqrt(2));
}

TEST(Point2, normalize) {
    putils::Point2f p{ 1, 0 };
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

TEST(Point3, normalize) {
    putils::Point3f p{ 1, 0, 0 };
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

TEST(Point2, normalized) {
    putils::Point2f p{ 1, 0 };
    const putils::Point2f p2 = putils::normalized(p);
    putils::normalize(p);
    EXPECT_TRUE(p == p2);

    p = { 100.f, 100.f };
    const putils::Point2f p3 = putils::normalized(p);
    putils::normalize(p);
    EXPECT_TRUE(p == p3);
}

TEST(Point3, normalized) {
    putils::Point3f p{ 1, 0, 0 };
    const putils::Point3f p2 = putils::normalized(p);
    putils::normalize(p);
    EXPECT_TRUE(p == p2);

    p = { 100.f, 100.f, 100.f };
    const putils::Point3f p3 = putils::normalized(p);
    putils::normalize(p);
    EXPECT_TRUE(p == p3);
}
