#include "angle.hpp"
#include "Point.hpp"

namespace putils {
	float constrainAngle(float angle) {
		angle = fmodf(angle + pi, pi * 2.f);
        if (angle < 0)
			angle += pi * 2.f;
        return angle - pi;
	}

	float getYawFromNormalizedDirection(const Vector3f & dir) {
		return -std::atan2f(dir.z, dir.x) + pi / 2.f;
	}

	float getPitchFromNormalizedDirection(const Vector3f & dir) {
		return -std::asinf(dir.y);
	}

	float get2DAngleFromNormalizedDirection(const Vector2f & dir) {
		return -std::atan2f(dir.y, dir.x) + pi / 2.f;
	}

	float toDegrees(float rads) {
		return rads * 180.f / pi;
	}

	float toRadians(float degs) {
		return degs * pi / 180.f;
	}
}