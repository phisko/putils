#pragma once

// putils
#include "point.hpp"

namespace putils {
	namespace directions {
		static const point2i up = { 0, 1 };
		static const point2i down = { 0, -1 };
		static const point2i left = { -1, 0 };
		static const point2i right = { 1, 0 };
		static const point2i none = { 0, 0 };
	}
}
