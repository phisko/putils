#pragma once

// putils
#include "Point.hpp"

namespace putils {
    static constexpr auto pi = 3.14159265359f;

	// constrains in [-PI, PI)
	PUTILS_CORE_EXPORT float constrainAngle(float angle) noexcept;

	PUTILS_CORE_EXPORT float getYawFromNormalizedDirection(const Vector3f & dir) noexcept;
	PUTILS_CORE_EXPORT float getPitchFromNormalizedDirection(const Vector3f & dir) noexcept;

	PUTILS_CORE_EXPORT float get2DAngleFromNormalizedDirection(const Vector2f & dir) noexcept;

	PUTILS_CORE_EXPORT float toDegrees(float rads) noexcept;
	PUTILS_CORE_EXPORT float toRadians(float degs) noexcept;
}