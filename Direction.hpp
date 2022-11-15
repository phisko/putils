#pragma once

// putils
#include "Point.hpp"

namespace putils {
    using Direction = Point2i;

    namespace Directions {
        static const Direction Up = { 0, 1 };
        static const Direction Down = { 0, -1 };
        static const Direction Left = { -1, 0 };
        static const Direction Right = { 1, 0 };
        static const Direction NoDirection = { 0, 0 };
    }
}
