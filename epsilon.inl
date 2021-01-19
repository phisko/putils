#include "epsilon.hpp"

namespace putils {
	template<typename T>
	bool epsilonEquals(T a, T b) noexcept {
		return std::abs(a - b) < std::numeric_limits<T>::epsilon();
	}
}