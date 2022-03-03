#include <gtest/gtest.h>
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
