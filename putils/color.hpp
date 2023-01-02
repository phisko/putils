#pragma once

// reflection
#include "putils/reflection.hpp"

namespace putils {
	struct PUTILS_CORE_EXPORT color {
		union {
			unsigned char attributes[4];
			struct {
				unsigned char a;
				unsigned char b;
				unsigned char g;
				unsigned char r;
			};
			unsigned int rgba;
			static_assert(sizeof(rgba) == sizeof(attributes));
		};

		explicit color(unsigned char r = 255, unsigned char g = 255, unsigned char b = 255, unsigned char a = 255) noexcept : r(r), g(g), b(b), a(a) {}
		color(const color & rhs) noexcept = default;
		color & operator=(const color & rhs) noexcept = default;
	};

#define refltype color
	putils_reflection_info {
		putils_reflection_class_name;
		putils_reflection_attributes(
			putils_reflection_attribute(r),
			putils_reflection_attribute(g),
			putils_reflection_attribute(b),
			putils_reflection_attribute(a)
		);
	};
#undef refltype

	struct PUTILS_CORE_EXPORT normalized_color {
		union {
			float attributes[4];
			struct {
				float r;
				float g;
				float b;
				float a;
			};
		};

		normalized_color(float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f) noexcept : r(r), g(g), b(b), a(a) {}
		normalized_color(const normalized_color & rhs) noexcept = default;
		normalized_color & operator=(const normalized_color & rhs) noexcept = default;
	};

#define refltype normalized_color
	putils_reflection_info {
		putils_reflection_class_name;
		putils_reflection_attributes(
			putils_reflection_attribute(r),
			putils_reflection_attribute(g),
			putils_reflection_attribute(b),
			putils_reflection_attribute(a)
		);
	};
#undef refltype

	/*
	** Conversion
	*/

	PUTILS_CORE_EXPORT color to_color(const normalized_color & normalized) noexcept;
	PUTILS_CORE_EXPORT normalized_color to_normalized_color(const color & color) noexcept;

	PUTILS_CORE_EXPORT unsigned int to_rgba(const normalized_color & color) noexcept;
	PUTILS_CORE_EXPORT normalized_color from_rgba(unsigned int color) noexcept;

	PUTILS_CORE_EXPORT unsigned int to_argb(const color & color) noexcept;
	PUTILS_CORE_EXPORT unsigned int to_argb(const normalized_color & color) noexcept;

	PUTILS_CORE_EXPORT color from_argb(unsigned int color) noexcept;

	/*
	** color * factor
	*/

	PUTILS_CORE_EXPORT color operator*(const color & color, float factor) noexcept;
	PUTILS_CORE_EXPORT color operator*(float factor, const color & color) noexcept;
	PUTILS_CORE_EXPORT color & operator*=(color & lhs, float factor) noexcept;

	/*
	** color / factor
	*/

	PUTILS_CORE_EXPORT color operator/(const color & color, float factor) noexcept;
	PUTILS_CORE_EXPORT color & operator/=(color & lhs, float factor) noexcept;

	/*
	** normalized_color * factor
	*/

	PUTILS_CORE_EXPORT normalized_color operator*(const normalized_color & color, float factor) noexcept;
	PUTILS_CORE_EXPORT normalized_color operator*(float factor, const normalized_color & color) noexcept;
	PUTILS_CORE_EXPORT normalized_color & operator*=(normalized_color & lhs, float factor) noexcept;

	/*
	** normalized_color / factor
	*/

	PUTILS_CORE_EXPORT normalized_color operator/(const normalized_color & color, float factor) noexcept;
	PUTILS_CORE_EXPORT normalized_color & operator/=(normalized_color & lhs, float factor) noexcept;

	/*
	** normalized_color * normalized_color
	*/

	PUTILS_CORE_EXPORT normalized_color operator*(const normalized_color & color, const normalized_color & rhs) noexcept;
	PUTILS_CORE_EXPORT normalized_color & operator*=(normalized_color & lhs, const normalized_color & rhs) noexcept;

	/*
	** normalized_color / normalized_color
	*/

	PUTILS_CORE_EXPORT normalized_color operator/(const normalized_color & color, const normalized_color & rhs) noexcept;
	PUTILS_CORE_EXPORT normalized_color & operator/=(normalized_color & lhs, const normalized_color & rhs) noexcept;
}