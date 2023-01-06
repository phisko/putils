#pragma once

// stl
#include <type_traits>

namespace putils {
	template<typename T>
	struct always_false : std::false_type {};
}

#define putils_static_assert_false(T) static_assert(putils::always_false<T>());