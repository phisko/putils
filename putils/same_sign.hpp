#pragma once

namespace putils {
	// Check if two values have the same sign
	template<typename ValueType>
	bool same_sign(ValueType a, ValueType b) noexcept {
		return a == 0 || b == 0 || ((a < 0) == (b < 0));
	}
}