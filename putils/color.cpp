#include "color.hpp"

namespace putils {
	color to_color(const normalized_color & normalized) noexcept {
		return color{ (unsigned char)(normalized.r * 255.f), (unsigned char)(normalized.g * 255.f), (unsigned char)(normalized.b * 255.f), (unsigned char)(normalized.a * 255.f) };
	}

	normalized_color to_normalized_color(const color & color) noexcept {
		return normalized_color{ (float)color.r / 255.f, (float)color.g / 255.f, (float)color.b / 255.f, (float)color.a / 255.f };
	}

	unsigned int to_rgba(const normalized_color & color) noexcept {
		return to_color(color).rgba;
	}

	normalized_color from_rgba(unsigned int rgba) noexcept {
		color c;
		c.rgba = rgba;
		return to_normalized_color(c);
	}

	unsigned int to_argb(const color & color) noexcept {
		return (color.a << 24) +
			(color.r << 16) +
			(color.g << 8) +
			color.b;
	}

	unsigned int to_argb(const normalized_color & color) noexcept {
		return to_argb(to_color(color));
	}

	color from_argb(unsigned int argb) noexcept {
		color c;
		c.a = (argb >> 24) & 255;
		c.r = (argb >> 16) & 255;
		c.g = (argb >> 8) & 255;
		c.b = argb & 255;
		return c;
	}

	/*
	** color * factor
	*/

	color operator*(const color & lhs, float rhs) noexcept {
		return color{ (unsigned char)(lhs.r * rhs), (unsigned char)(lhs.g * rhs), (unsigned char)(lhs.b * rhs), lhs.a };
	}

	color operator*(float lhs, const color & rhs) noexcept {
		return color{ (unsigned char)(rhs.r * lhs), (unsigned char)(rhs.g * lhs), (unsigned char)(rhs.b * lhs), rhs.a };
	}

	color & operator*=(color & lhs, float rhs) noexcept {
		lhs.r = (unsigned char)(lhs.r * rhs);
		lhs.g = (unsigned char)(lhs.g * rhs);
		lhs.b = (unsigned char)(lhs.b * rhs);
		return lhs;
	}

	/*
	** color / factor
	*/

	color operator/(const color & lhs, float rhs) noexcept {
		return color{ (unsigned char)(lhs.r / rhs), (unsigned char)(lhs.g / rhs), (unsigned char)(lhs.b / rhs), lhs.a };
	}

	color & operator/=(color & lhs, float factor) noexcept {
		lhs.r = (unsigned char)(lhs.r / factor);
		lhs.g = (unsigned char)(lhs.g / factor);
		lhs.b = (unsigned char)(lhs.b / factor);
		return lhs;
	}

	/*
	** normalized_color * factor
	*/

	normalized_color operator*(const normalized_color & lhs, float rhs) noexcept {
		return normalized_color{ lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a };
	}

	normalized_color operator*(float lhs, const normalized_color & rhs) noexcept {
		return normalized_color{ rhs.r * lhs, rhs.g * lhs, rhs.b * lhs, rhs.a };
	}

	normalized_color & operator*=(normalized_color & lhs, float rhs) noexcept {
		lhs.r *= rhs;
		lhs.g *= rhs;
		lhs.b *= rhs;
		return lhs;
	}

	/*
	** normalized_color / factor
	*/

	normalized_color operator/(const normalized_color & lhs, float rhs) noexcept {
		return normalized_color{ lhs.r / rhs, lhs.g / rhs, lhs.b / rhs, lhs.a };
	}

	normalized_color & operator/=(normalized_color & lhs, float rhs) noexcept {
		lhs.r /= rhs;
		lhs.g /= rhs;
		lhs.b /= rhs;
		return lhs;
	}

	/*
	** normalized_color * normalized_color
	*/

	normalized_color operator*(const normalized_color & lhs, const normalized_color & rhs) noexcept {
		return normalized_color{ lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b, lhs.a * rhs.a };
	}

	normalized_color & operator*=(normalized_color & lhs, const normalized_color & rhs) noexcept {
		lhs.r *= rhs.r;
		lhs.g *= rhs.g;
		lhs.b *= rhs.b;
		lhs.a *= rhs.a;
		return lhs;
	}

	/*
	** normalized_color / normalized_color
	*/

	normalized_color operator/(const normalized_color & lhs, const normalized_color & rhs) noexcept {
		return normalized_color{ lhs.r / rhs.r, lhs.g / rhs.g, lhs.b / rhs.b, lhs.a / rhs.a };
	}

	normalized_color & operator/=(normalized_color & lhs, const normalized_color & rhs) noexcept {
		lhs.r /= rhs.r;
		lhs.g /= rhs.g;
		lhs.b /= rhs.b;
		lhs.a /= rhs.a;
		return lhs;
	}
}
