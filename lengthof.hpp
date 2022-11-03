#pragma once

// stl
#include <type_traits>

namespace putils {
	template<typename T>
	constexpr size_t lengthof() noexcept {
		return sizeof(T) / sizeof(std::remove_all_extents_t<T>);
	}

	template<typename T, size_t N>
	constexpr size_t lengthof(const T(&)[N]) noexcept {
		return N;
	}
}
