#pragma once

#include <math.h>
#include "Point.hpp"

namespace putils {
    static constexpr auto pi = 3.14159265359f;

	// constrains in [-PI, PI)
	float constrainAngle(float angle);

	float getYawFromNormalizedDirection(const Vector3f & dir);
	float getPitchFromNormalizedDirection(const Vector3f & dir);

	float get2DAngleFromNormalizedDirection(const Vector2f & dir);

	float toDegrees(float rads);
	float toRadians(float degs);
}