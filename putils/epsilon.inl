#include "epsilon.hpp"

namespace putils {
	template<typename T>
	bool epsilon_equals(T a, T b) noexcept {
		return std::abs(a - b) < std::numeric_limits<T>::epsilon();
	}
}