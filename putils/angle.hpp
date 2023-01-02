#pragma once

// putils
#include "point.hpp"

namespace putils {
	static constexpr auto pi = 3.14159265359f;

	// constrains in [-PI, PI)
	PUTILS_CORE_EXPORT float constrain_angle(float angle) noexcept;

	PUTILS_CORE_EXPORT float get_yaw_from_normalized_direction(const vec3f & dir) noexcept;
	PUTILS_CORE_EXPORT float get_pitch_from_normalized_direction(const vec3f & dir) noexcept;

	PUTILS_CORE_EXPORT float get_2d_angle_from_normalized_direction(const vec2f & dir) noexcept;

	PUTILS_CORE_EXPORT float to_degrees(float rads) noexcept;
	PUTILS_CORE_EXPORT float to_radians(float degs) noexcept;
}