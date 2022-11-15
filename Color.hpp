#pragma once

// reflection
#include "reflection.hpp"

namespace putils {
	struct PUTILS_CORE_EXPORT Color {
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

		explicit Color(unsigned char r = 255, unsigned char g = 255, unsigned char b = 255, unsigned char a = 255) noexcept : r(r), g(g), b(b), a(a) {}
		Color(const Color & rhs) noexcept = default;
		Color & operator=(const Color & rhs) noexcept = default;
	};

#define refltype Color
	putils_reflection_info{
		putils_reflection_class_name;
		putils_reflection_attributes(
			putils_reflection_attribute(r),
			putils_reflection_attribute(g),
			putils_reflection_attribute(b),
			putils_reflection_attribute(a)
		);
	};
#undef refltype

	struct PUTILS_CORE_EXPORT NormalizedColor {
		union {
			float attributes[4];
			struct {
				float r;
				float g;
				float b;
				float a;
			};
		};

		NormalizedColor(float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f) noexcept : r(r), g(g), b(b), a(a) {}
		NormalizedColor(const NormalizedColor & rhs) noexcept = default;
		NormalizedColor & operator=(const NormalizedColor & rhs) noexcept = default;
	};

#define refltype NormalizedColor
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

	PUTILS_CORE_EXPORT Color toColor(const NormalizedColor & normalized) noexcept;
	PUTILS_CORE_EXPORT NormalizedColor toNormalizedColor(const Color & color) noexcept;

	PUTILS_CORE_EXPORT unsigned int toRGBA(const NormalizedColor & color) noexcept;
	PUTILS_CORE_EXPORT NormalizedColor fromRGBA(unsigned int color) noexcept;

	PUTILS_CORE_EXPORT unsigned int toARGB(const Color & color) noexcept;
	PUTILS_CORE_EXPORT unsigned int toARGB(const NormalizedColor & color) noexcept;

	PUTILS_CORE_EXPORT Color fromARGB(unsigned int color) noexcept;

	/*
	** Color * factor
	*/

	PUTILS_CORE_EXPORT Color operator*(const Color & color, float factor) noexcept;
	PUTILS_CORE_EXPORT Color operator*(float factor, const Color & color) noexcept;
	PUTILS_CORE_EXPORT Color & operator*=(Color & lhs, float factor) noexcept;

	/*
	** Color / factor
	*/

	PUTILS_CORE_EXPORT Color operator/(const Color & color, float factor) noexcept;
	PUTILS_CORE_EXPORT Color & operator/=(Color & lhs, float factor) noexcept;

	/*
	** NormalizedColor * factor
	*/

	PUTILS_CORE_EXPORT NormalizedColor operator*(const NormalizedColor & color, float factor) noexcept;
	PUTILS_CORE_EXPORT NormalizedColor operator*(float factor, const NormalizedColor & color) noexcept;
	PUTILS_CORE_EXPORT NormalizedColor & operator*=(NormalizedColor & lhs, float factor) noexcept;

	/*
	** NormalizedColor / factor
	*/

	PUTILS_CORE_EXPORT NormalizedColor operator/(const NormalizedColor & color, float factor) noexcept;
	PUTILS_CORE_EXPORT NormalizedColor & operator/=(NormalizedColor & lhs, float factor) noexcept;

	/*
	** NormalizedColor * NormalizedColor
	*/

	PUTILS_CORE_EXPORT NormalizedColor operator*(const NormalizedColor & color, const NormalizedColor & rhs) noexcept;
	PUTILS_CORE_EXPORT NormalizedColor & operator*=(NormalizedColor & lhs, const NormalizedColor & rhs) noexcept;

	/*
	** NormalizedColor / NormalizedColor
	*/

	PUTILS_CORE_EXPORT NormalizedColor operator/(const NormalizedColor & color, const NormalizedColor & rhs) noexcept;
	PUTILS_CORE_EXPORT NormalizedColor & operator/=(NormalizedColor & lhs, const NormalizedColor & rhs) noexcept;
}