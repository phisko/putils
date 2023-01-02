#pragma once

// putils
#include "point.hpp"

namespace putils {
	template<typename Precision, size_t Dimensions>
	struct rect {
		using my_point = point<Precision, Dimensions>;
		my_point position;
		my_point size;
	};

	template<typename T, size_t D>
#define refltype rect<T, D>
	putils_reflection_info_template {
		static constexpr const char class_name_priv[] = { 'r', 'e', 'c', 't', '0' + D, 0 };
		static constexpr const char * class_name = class_name_priv;

		putils_reflection_attributes(
			putils_reflection_attribute(position),
			putils_reflection_attribute(size)
		);

		putils_reflection_used_types(
			putils_reflection_type(typename refltype::my_point)
		);
	};
#undef refltype

	using rect3d = rect<double, 3>;
	using rect2d = rect<double, 2>;

	using rect3i = rect<int, 3>;
	using rect2i = rect<int, 2>;

	using rect3ui = rect<unsigned int, 3>;
	using rect2ui = rect<unsigned int, 2>;

	using rect3f = rect<float, 3>;
	using rect2f = rect<float, 2>;

	template<typename P1, typename P2, size_t D>
	bool operator==(const rect<P1, D> & lhs, const rect<P2, D> & rhs) noexcept;

	template<typename P1, typename P2, size_t D>
	bool operator!=(const rect<P1, D> & lhs, const rect<P2, D> & rhs) noexcept;
}

#include "rect.inl"