#pragma once

// stl
#include <type_traits>

namespace putils {
	template<typename T>
	struct always_false : std::false_type {};
}
