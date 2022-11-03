// gtest
#include <gtest/gtest.h>

// putils
#include "angle.hpp"

TEST(angle, constrainAngle) {
    float angle = putils::pi * 2.f;
    EXPECT_NEAR(putils::constrainAngle(angle), 0.f, .01f);

    angle = putils::pi;
    EXPECT_NEAR(putils::constrainAngle(angle), -putils::pi, .01f);
}

TEST(angle, getYawFromNormalizedDirection) {
    putils::Vector3f dir = { 1.f, 0.f, 0.f };
    float angle = putils::getYawFromNormalizedDirection(dir);
    EXPECT_NEAR(angle, putils::pi / 2.f, .01f);

    dir = { -1.f, 0.f, 0.f };
    angle = putils::getYawFromNormalizedDirection(dir);
    EXPECT_NEAR(angle, -putils::pi / 2.f, .01f);

    dir = { 0.f, 0.f, 1.f };
    angle = putils::getYawFromNormalizedDirection(dir);
    EXPECT_NEAR(angle, 0.f, .01f);

    dir = { 0.f, 0.f, -1.f };
    angle = putils::getYawFromNormalizedDirection(dir);
    EXPECT_NEAR(angle, putils::pi, .01f);
}

TEST(angle, getPitchFromNormalizedDirection) {
    putils::Vector3f dir = { 0.f, 0.f, 0.f };
    float angle = putils::getPitchFromNormalizedDirection(dir);
    EXPECT_NEAR(angle, 0.f, .01f);

    dir = { 0.f, 1.f, 0.f };
    angle = putils::getPitchFromNormalizedDirection(dir);
    EXPECT_NEAR(angle, putils::pi / 2.f, .01f);

    dir = { 0.f, -1.f, 0.f };
    angle = putils::getPitchFromNormalizedDirection(dir);
    EXPECT_NEAR(angle, -putils::pi / 2.f, .01f);
}

TEST(angle, get2DAngleFromNormalizedDirection) {
    putils::Vector2f dir = { 1.f, 0.f };
    float angle = putils::get2DAngleFromNormalizedDirection(dir);
    EXPECT_NEAR(angle, putils::pi / 2.f, .01f);

    dir = { -1.f, 0.f };
    angle = putils::get2DAngleFromNormalizedDirection(dir);
    EXPECT_NEAR(angle, -putils::pi / 2.f, .01f);

    dir = { 0.f, 1.f };
    angle = putils::get2DAngleFromNormalizedDirection(dir);
    EXPECT_NEAR(angle, 0.f, .01f);

    dir = { 0.f, -1.f };
    angle = putils::get2DAngleFromNormalizedDirection(dir);
    EXPECT_NEAR(angle, putils::pi, .01f);
}

TEST(angle, toDegrees) {
    EXPECT_NEAR(putils::toDegrees(0.f), 0.f, .01f);

    EXPECT_NEAR(putils::toDegrees(putils::pi), 180.f, .01f);
    EXPECT_NEAR(putils::toDegrees(-putils::pi), -180.f, .01f);

    EXPECT_NEAR(putils::toDegrees(putils::pi / 2.f), 90.f, .01f);
    EXPECT_NEAR(putils::toDegrees(-putils::pi / 2.f), -90.f, .01f);

    EXPECT_NEAR(putils::toDegrees(putils::pi * 2.f), 360.f, .01f);
    EXPECT_NEAR(putils::toDegrees(-putils::pi * 2.f), -360.f, .01f);
}

TEST(angle, toRadians) {
    EXPECT_NEAR(putils::toRadians(0.f), 0.f, .01f);

    EXPECT_NEAR(putils::toRadians(180.f), putils::pi, .01f);
    EXPECT_NEAR(putils::toRadians(-180.f), -putils::pi, .01f);

    EXPECT_NEAR(putils::toRadians(90.f), putils::pi / 2.f, .01f);
    EXPECT_NEAR(putils::toRadians(-90.f), -putils::pi / 2.f, .01f);

    EXPECT_NEAR(putils::toRadians(360.f), putils::pi * 2.f, .01f);
    EXPECT_NEAR(putils::toRadians(-360.f), -putils::pi * 2.f, .01f);
}
