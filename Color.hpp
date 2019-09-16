#pragma once

#include "reflection/Reflectible.hpp"

namespace putils {
	struct Color {
		union {
			unsigned char attributes[4];
			struct {
				unsigned char r;
				unsigned char g;
				unsigned char b;
				unsigned char a;
			};
			unsigned int rgba;
		};

		pmeta_get_class_name(Color);
		pmeta_get_attributes(
			pmeta_reflectible_attribute(&Color::r),
			pmeta_reflectible_attribute(&Color::g),
			pmeta_reflectible_attribute(&Color::b),
			pmeta_reflectible_attribute(&Color::a)
		);

		Color(unsigned char r = 255, unsigned char g = 255, unsigned char b = 255, unsigned char a = 255) : r(r), g(g), b(b), a(a) {}
		Color(const Color & rhs) = default;
		Color & operator=(const Color & rhs) = default;
	};

	struct NormalizedColor {
		union {
			float attributes[4];
			struct {
				float r;
				float g;
				float b;
				float a;
			};
		};

		pmeta_get_class_name(NormalizedColor);
		pmeta_get_attributes(
			pmeta_reflectible_attribute(&NormalizedColor::r),
			pmeta_reflectible_attribute(&NormalizedColor::g),
			pmeta_reflectible_attribute(&NormalizedColor::b),
			pmeta_reflectible_attribute(&NormalizedColor::a)
		);
		NormalizedColor(float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f) : r(r), g(g), b(b), a(a) {}
		NormalizedColor(const NormalizedColor & rhs) = default;
		NormalizedColor & operator=(const NormalizedColor & rhs) = default;
	};

	/*
	** Conversion
	*/

	inline Color toColor(const NormalizedColor & normalized) {
		return { (unsigned char)(normalized.r * 255.f), (unsigned char)(normalized.g * 255.f), (unsigned char)(normalized.b * 255.f), (unsigned char)(normalized.a * 255.f) };
	}

	inline NormalizedColor toNormalizedColor(const Color & color) {
		return { (float)color.r / 255.f, (float)color.g / 255.f, (float)color.b / 255.f, (float)color.a / 255.f };
	}

	inline unsigned int toRGBA(const NormalizedColor & color) {
		return toColor(color).rgba;
	}

	inline NormalizedColor fromRGBA(unsigned int color) {
		Color c;
		c.rgba = color;
		return toNormalizedColor(color);
	}

	inline unsigned int toARGB(const Color & color) {
		return (color.a << 24) +
			(color.r << 16) +
			(color.g << 8) +
			color.b;
	}

	inline unsigned int toARGB(const NormalizedColor & color) {
		return toARGB(toColor(color));
	}

	inline Color fromARGB(unsigned int color) {
		Color c;
		c.a = (color >> 24) & 255;
		c.r = (color >> 16) & 255;
		c.g = (color >> 8) & 255;
		c.b = color & 255;
		return c;
	}

	/*
	** Color * factor
	*/

	inline Color operator*(const Color & color, float factor) {
		return { (unsigned char)(color.r * factor), (unsigned char)(color.g * factor), (unsigned char)(color.b * factor), color.a };
	}
	inline Color operator*(float factor, const Color & color) {
		return { (unsigned char)(color.r * factor), (unsigned char)(color.g * factor), (unsigned char)(color.b * factor), color.a };
	}
	inline Color & operator*=(Color & lhs, float factor) {
		lhs.r = (unsigned char)(lhs.r * factor);
		lhs.g = (unsigned char)(lhs.g * factor);
		lhs.b = (unsigned char)(lhs.b * factor);
		return lhs;
	}

	/*
	** Color / factor
	*/

	inline Color operator/(const Color & color, float factor) {
		return { (unsigned char)(color.r / factor), (unsigned char)(color.g / factor), (unsigned char)(color.b / factor), color.a };
	}
	inline Color & operator/=(Color & lhs, float factor) {
		lhs.r = (unsigned char)(lhs.r / factor);
		lhs.g = (unsigned char)(lhs.g / factor);
		lhs.b = (unsigned char)(lhs.b / factor);
		return lhs;
	}

	/*
	** NormalizedColor * factor
	*/

	inline NormalizedColor operator*(const NormalizedColor & color, float factor) {
		return { color.r * factor, color.g * factor, color.b * factor, color.a };
	}
	inline NormalizedColor operator*(float factor, const NormalizedColor & color) {
		return { color.r * factor, color.g * factor, color.b * factor, color.a };
	}
	inline NormalizedColor & operator*=(NormalizedColor & lhs, float factor) {
		lhs.r *= factor; lhs.g *= factor; lhs.b *= factor;
		return lhs;
	}

	/*
	** NormalizedColor / factor
	*/

	inline NormalizedColor operator/(const NormalizedColor & color, float factor) {
		return { color.r / factor, color.g / factor, color.b / factor, color.a };
	}
	inline NormalizedColor & operator/=(NormalizedColor & lhs, float factor) {
		lhs.r /= factor; lhs.g /= factor; lhs.b /= factor;
		return lhs;
	}

	/*
	** NormalizedColor * NormalizedColor
	*/

	inline NormalizedColor operator*(const NormalizedColor & color, const NormalizedColor & rhs) {
		return { color.r * rhs.r, color.g * rhs.g, color.b * rhs.b, color.a * rhs.a };
	}
	inline NormalizedColor & operator*=(NormalizedColor & lhs, const NormalizedColor & rhs) {
		lhs.r *= rhs.r; lhs.g *= rhs.g; lhs.b *= rhs.b; lhs.a *= rhs.a;
		return lhs;
	}

	/*
	** NormalizedColor / NormalizedColor
	*/

	inline NormalizedColor operator/(const NormalizedColor & color, const NormalizedColor & rhs) {
		return { color.r / rhs.r, color.g / rhs.g, color.b / rhs.b, color.a / rhs.a };
	}
	inline NormalizedColor & operator/=(NormalizedColor & lhs, const NormalizedColor & rhs) {
		lhs.r /= rhs.r; lhs.g /= rhs.g; lhs.b /= rhs.b; lhs.a /= rhs.a;
		return lhs;
	}
}