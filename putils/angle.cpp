#include "angle.hpp"

// stl
#include <cmath>

namespace putils {
	float constrain_angle(float angle) noexcept {
		angle = fmodf(angle + pi, pi * 2.f);
        if (angle < 0)
			angle += pi * 2.f;
        return angle - pi;
	}

	float get_yaw_from_normalized_direction(const vec3f & dir) noexcept {
		return -atan2f(dir.z, dir.x) + pi / 2.f;
	}

	float get_pitch_from_normalized_direction(const vec3f & dir) noexcept {
		return asinf(dir.y);
	}

	float get_2d_angle_from_normalized_direction(const vec2f & dir) noexcept {
		return -atan2f(dir.y, dir.x) + pi / 2.f;
	}

	float to_degrees(float rads) noexcept {
		return rads * 180.f / pi;
	}

	float to_radians(float degs) noexcept {
		return degs * pi / 180.f;
	}
}