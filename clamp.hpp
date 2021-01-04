#pragma once

namespace putils {
	template<typename T>
	T clamp(T val, T min, T max) noexcept {
		val = std::max(min, val);
		val = std::min(max, val);
		return val;
	}
}