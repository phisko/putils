#pragma once

namespace putils {
	template<typename T>
	T clamp(T val, T min, T max) {
		val = std::max(min, val);
		val = std::min(max, val);
		return val;
	}
}