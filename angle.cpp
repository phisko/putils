#include "angle.hpp"

#include <cmath>
#include "Point.hpp"

namespace putils {
	float constrainAngle(float angle) noexcept {
		angle = fmodf(angle + pi, pi * 2.f);
        if (angle < 0)
			angle += pi * 2.f;
        return angle - pi;
	}

	float getYawFromNormalizedDirection(const Vector3f & dir) noexcept {
		return -atan2f(dir.z, dir.x) + pi / 2.f;
	}

	float getPitchFromNormalizedDirection(const Vector3f & dir) noexcept {
		return -asinf(dir.y);
	}

	float get2DAngleFromNormalizedDirection(const Vector2f & dir) noexcept {
		return -atan2f(dir.y, dir.x) + pi / 2.f;
	}

	float toDegrees(float rads) noexcept {
		return rads * 180.f / pi;
	}

	float toRadians(float degs) noexcept {
		return degs * pi / 180.f;
	}
}