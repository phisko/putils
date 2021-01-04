#include "Rect.hpp"

namespace putils {
	template<typename P1, typename P2, size_t D>
	bool operator==(const Rect<P1, D> & lhs, const Rect<P2, D> & rhs) noexcept {
		return lhs.position == rhs.position && lhs.size == rhs.size;
	}

	template<typename P1, typename P2, size_t D>
	bool operator!=(const Rect<P1, D> & lhs, const Rect<P2, D> & rhs) noexcept {
		return !(lhs == rhs);
	}
}