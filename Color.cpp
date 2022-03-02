#include "Color.hpp"

namespace putils {
	Color toColor(const NormalizedColor & normalized) noexcept {
		return Color{ (unsigned char)(normalized.r * 255.f), (unsigned char)(normalized.g * 255.f), (unsigned char)(normalized.b * 255.f), (unsigned char)(normalized.a * 255.f) };
	}

	NormalizedColor toNormalizedColor(const Color & color) noexcept {
		return NormalizedColor{ (float)color.r / 255.f, (float)color.g / 255.f, (float)color.b / 255.f, (float)color.a / 255.f };
	}

	unsigned int toRGBA(const NormalizedColor & color) noexcept {
		return toColor(color).rgba;
	}

	NormalizedColor fromRGBA(unsigned int color) noexcept {
		Color c;
		c.rgba = color;
		return toNormalizedColor(c);
	}

	unsigned int toARGB(const Color & color) noexcept {
		return (color.a << 24) +
			(color.r << 16) +
			(color.g << 8) +
			color.b;
	}

	unsigned int toARGB(const NormalizedColor & color) noexcept {
		return toARGB(toColor(color));
	}

	Color fromARGB(unsigned int color) noexcept {
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

	Color operator*(const Color & color, float factor) noexcept {
		return Color{ (unsigned char)(color.r * factor), (unsigned char)(color.g * factor), (unsigned char)(color.b * factor), color.a };
	}

	Color operator*(float factor, const Color & color) noexcept {
		return Color{ (unsigned char)(color.r * factor), (unsigned char)(color.g * factor), (unsigned char)(color.b * factor), color.a };
	}

	Color & operator*=(Color & lhs, float factor) noexcept {
		lhs.r = (unsigned char)(lhs.r * factor);
		lhs.g = (unsigned char)(lhs.g * factor);
		lhs.b = (unsigned char)(lhs.b * factor);
		return lhs;
	}

	/*
	** Color / factor
	*/

	Color operator/(const Color & color, float factor) noexcept {
		return Color{ (unsigned char)(color.r / factor), (unsigned char)(color.g / factor), (unsigned char)(color.b / factor), color.a };
	}

	Color & operator/=(Color & lhs, float factor) noexcept {
		lhs.r = (unsigned char)(lhs.r / factor);
		lhs.g = (unsigned char)(lhs.g / factor);
		lhs.b = (unsigned char)(lhs.b / factor);
		return lhs;
	}

	/*
	** NormalizedColor * factor
	*/

	NormalizedColor operator*(const NormalizedColor & color, float factor) noexcept {
		return NormalizedColor{ color.r * factor, color.g * factor, color.b * factor, color.a };
	}

	NormalizedColor operator*(float factor, const NormalizedColor & color) noexcept {
		return NormalizedColor{ color.r * factor, color.g * factor, color.b * factor, color.a };
	}

	NormalizedColor & operator*=(NormalizedColor & lhs, float factor) noexcept {
		lhs.r *= factor; lhs.g *= factor; lhs.b *= factor;
		return lhs;
	}

	/*
	** NormalizedColor / factor
	*/

	NormalizedColor operator/(const NormalizedColor & color, float factor) noexcept {
		return NormalizedColor{ color.r / factor, color.g / factor, color.b / factor, color.a };
	}

	NormalizedColor & operator/=(NormalizedColor & lhs, float factor) noexcept {
		lhs.r /= factor; lhs.g /= factor; lhs.b /= factor;
		return lhs;
	}

	/*
	** NormalizedColor * NormalizedColor
	*/

	NormalizedColor operator*(const NormalizedColor & color, const NormalizedColor & rhs) noexcept {
		return NormalizedColor{ color.r * rhs.r, color.g * rhs.g, color.b * rhs.b, color.a * rhs.a };
	}

	NormalizedColor & operator*=(NormalizedColor & lhs, const NormalizedColor & rhs) noexcept {
		lhs.r *= rhs.r; lhs.g *= rhs.g; lhs.b *= rhs.b; lhs.a *= rhs.a;
		return lhs;
	}

	/*
	** NormalizedColor / NormalizedColor
	*/

	NormalizedColor operator/(const NormalizedColor & color, const NormalizedColor & rhs) noexcept {
		return NormalizedColor{ color.r / rhs.r, color.g / rhs.g, color.b / rhs.b, color.a / rhs.a };
	}

	NormalizedColor & operator/=(NormalizedColor & lhs, const NormalizedColor & rhs) noexcept {
		lhs.r /= rhs.r; lhs.g /= rhs.g; lhs.b /= rhs.b; lhs.a /= rhs.a;
		return lhs;
	}
}
