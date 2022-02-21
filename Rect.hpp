#pragma once

#include "Point.hpp"

namespace putils {
    template<typename Precision, size_t Dimensions>
	struct Rect {
		using Point = Point<Precision, Dimensions>;

		Point position;
		Point size;
	};

	template<typename T, size_t D>
#define refltype Rect<T, D>
	putils_reflection_info_template{
		static constexpr char class_name[] = { 'R', 'e', 'c', 't', '0' + D, 0 };

		putils_reflection_attributes(
			putils_reflection_attribute(position),
			putils_reflection_attribute(size)
		);

		putils_reflection_used_types(
			putils_reflection_type(typename refltype::Point)
		);
	};
#undef refltype

    using Rect3d = Rect<double, 3>;
    using Rect2d = Rect<double, 2>;

    using Rect3i = Rect<int, 3>;
    using Rect2i = Rect<int, 2>;

    using Rect3ui = Rect<unsigned int, 3>;
    using Rect2ui = Rect<unsigned int, 2>;

    using Rect3f = Rect<float, 3>;
    using Rect2f = Rect<float, 2>;

	template<typename P1, typename P2, size_t D>
	bool operator==(const Rect<P1, D> & lhs, const Rect<P2, D> & rhs) noexcept;

	template<typename P1, typename P2, size_t D>
	bool operator!=(const Rect<P1, D> & lhs, const Rect<P2, D> & rhs) noexcept;
}

#include "Rect.inl"