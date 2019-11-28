#pragma once

#include <math.h>

namespace putils {
    static constexpr auto pi = 3.14159265359f;

    static float constrainAngle(float angle) {
        angle = fmodf(angle + pi, pi * 2.f);
        if (angle < 0)
        angle += pi * 2.f;
        return angle - pi;
    }
}