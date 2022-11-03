#pragma once

// putils
#include "Point.hpp"

namespace putils {
    static constexpr auto pi = 3.14159265359f;

	// constrains in [-PI, PI)
	float constrainAngle(float angle) noexcept;

	float getYawFromNormalizedDirection(const Vector3f & dir) noexcept;
	float getPitchFromNormalizedDirection(const Vector3f & dir) noexcept;

	float get2DAngleFromNormalizedDirection(const Vector2f & dir) noexcept;

	float toDegrees(float rads) noexcept;
	float toRadians(float degs) noexcept;
}