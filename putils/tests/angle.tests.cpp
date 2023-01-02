// gtest
#include <gtest/gtest.h>

// putils
#include "putils/angle.hpp"

TEST(angle, constrain_angle) {
	float angle = putils::pi * 2.f;
	EXPECT_NEAR(putils::constrain_angle(angle), 0.f, .01f);

	angle = putils::pi;
	EXPECT_NEAR(putils::constrain_angle(angle), -putils::pi, .01f);
}

TEST(angle, get_yaw_from_normalized_direction) {
	putils::vec3f dir = { 1.f, 0.f, 0.f };
	float angle = putils::get_yaw_from_normalized_direction(dir);
	EXPECT_NEAR(angle, putils::pi / 2.f, .01f);

	dir = { -1.f, 0.f, 0.f };
	angle = putils::get_yaw_from_normalized_direction(dir);
	EXPECT_NEAR(angle, -putils::pi / 2.f, .01f);

	dir = { 0.f, 0.f, 1.f };
	angle = putils::get_yaw_from_normalized_direction(dir);
	EXPECT_NEAR(angle, 0.f, .01f);

	dir = { 0.f, 0.f, -1.f };
	angle = putils::get_yaw_from_normalized_direction(dir);
	EXPECT_NEAR(angle, putils::pi, .01f);
}

TEST(angle, get_pitch_from_normalized_direction) {
	putils::vec3f dir = { 0.f, 0.f, 0.f };
	float angle = putils::get_pitch_from_normalized_direction(dir);
	EXPECT_NEAR(angle, 0.f, .01f);

	dir = { 0.f, 1.f, 0.f };
	angle = putils::get_pitch_from_normalized_direction(dir);
	EXPECT_NEAR(angle, putils::pi / 2.f, .01f);

	dir = { 0.f, -1.f, 0.f };
	angle = putils::get_pitch_from_normalized_direction(dir);
	EXPECT_NEAR(angle, -putils::pi / 2.f, .01f);
}

TEST(angle, get_2d_angle_from_normalized_direction) {
	putils::vec2f dir = { 1.f, 0.f };
	float angle = putils::get_2d_angle_from_normalized_direction(dir);
	EXPECT_NEAR(angle, putils::pi / 2.f, .01f);

	dir = { -1.f, 0.f };
	angle = putils::get_2d_angle_from_normalized_direction(dir);
	EXPECT_NEAR(angle, -putils::pi / 2.f, .01f);

	dir = { 0.f, 1.f };
	angle = putils::get_2d_angle_from_normalized_direction(dir);
	EXPECT_NEAR(angle, 0.f, .01f);

	dir = { 0.f, -1.f };
	angle = putils::get_2d_angle_from_normalized_direction(dir);
	EXPECT_NEAR(angle, putils::pi, .01f);
}

TEST(angle, to_degrees) {
	EXPECT_NEAR(putils::to_degrees(0.f), 0.f, .01f);

	EXPECT_NEAR(putils::to_degrees(putils::pi), 180.f, .01f);
	EXPECT_NEAR(putils::to_degrees(-putils::pi), -180.f, .01f);

	EXPECT_NEAR(putils::to_degrees(putils::pi / 2.f), 90.f, .01f);
	EXPECT_NEAR(putils::to_degrees(-putils::pi / 2.f), -90.f, .01f);

	EXPECT_NEAR(putils::to_degrees(putils::pi * 2.f), 360.f, .01f);
	EXPECT_NEAR(putils::to_degrees(-putils::pi * 2.f), -360.f, .01f);
}

TEST(angle, to_radians) {
	EXPECT_NEAR(putils::to_radians(0.f), 0.f, .01f);

	EXPECT_NEAR(putils::to_radians(180.f), putils::pi, .01f);
	EXPECT_NEAR(putils::to_radians(-180.f), -putils::pi, .01f);

	EXPECT_NEAR(putils::to_radians(90.f), putils::pi / 2.f, .01f);
	EXPECT_NEAR(putils::to_radians(-90.f), -putils::pi / 2.f, .01f);

	EXPECT_NEAR(putils::to_radians(360.f), putils::pi * 2.f, .01f);
	EXPECT_NEAR(putils::to_radians(-360.f), -putils::pi * 2.f, .01f);
}
