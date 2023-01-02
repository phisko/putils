#include "point.hpp"

// stl
#include <cmath>

namespace putils {
#define TemplateDecl template<typename Precision>
#define TPoint point<Precision, 2>
	// clang-format off

	TemplateDecl
	TPoint::point(Precision x, Precision y) noexcept
		: x(x),
		  y(y) {}

	TemplateDecl
	TPoint::point(const Precision * p) noexcept
		: x(p[0]),
		  y(p[1]) {}

	TemplateDecl
	template<typename P>
	TPoint::point(const point<P, 2> & other) noexcept
		: x((Precision)other.x),
		  y((Precision)other.y) {}

	TemplateDecl
	template<typename P>
	TPoint & TPoint::operator=(const point<P, 2> & other) noexcept {
		x = (Precision)other.x;
		y = (Precision)other.y;
		return *this;
	}

	TemplateDecl
	template<typename P>
	TPoint::point(const point<P, 3> & other) noexcept
		: x((Precision)other.x),
		  y((Precision)other.y) {}

	TemplateDecl
	template<typename P>
	TPoint & TPoint::operator=(const point<P, 3> & other) noexcept {
		x = (Precision)other.x;
		y = (Precision)other.y;
		return *this;
	}

	TemplateDecl
	Precision & TPoint::operator[](size_t index) noexcept {
		return raw[index];
	}

	TemplateDecl
	Precision TPoint::operator[](size_t index) const noexcept {
		return raw[index];
	}

#undef TPoint

#define TPoint point<Precision, 3>
	TemplateDecl
	TPoint::point(Precision x, Precision y, Precision z) noexcept
		: x(x), y(y), z(z) {}

	TemplateDecl
	TPoint::point(const Precision * p) noexcept
		: x(p[0]), y(p[1]), z(p[2]) {}

	TemplateDecl
	template<typename P>
	TPoint::point(const point<P, 2> & other) noexcept
		: x((Precision)other.x),
		  y((Precision)other.y),
		  z(0) {}

	TemplateDecl
	template<typename P>
	TPoint & TPoint::operator=(const point<P, 2> & other) noexcept {
		x = (Precision)other.x;
		y = (Precision)other.y;
		z = 0;
		return *this;
	}

	TemplateDecl
	template<typename P>
	TPoint::point(const point<P, 3> & other) noexcept
		: x((Precision)other.x), y((Precision)other.y), z((Precision)other.z) {}

	TemplateDecl
	template<typename P>
	TPoint & TPoint::operator=(const point<P, 3> & other) noexcept {
		x = (Precision)other.x;
		y = (Precision)other.y;
		z = (Precision)other.z;
		return *this;
	}

	TemplateDecl
	Precision & TPoint::operator[](size_t index) noexcept {
		return raw[index];
	}

	TemplateDecl
	Precision TPoint::operator[](size_t index) const noexcept {
		return raw[index];
	}

#undef TPoint
#undef TemplateDecl

#define TemplateDecl template<typename P1, typename P2, size_t D>
#define Point1 point<P1, D>
#define point2 point<P2, D>

	TemplateDecl
	bool operator==(const Point1 & lhs, const point2 & rhs) noexcept {
		for (size_t i = 0; i < D; ++i) {
			if (lhs[i] != rhs[i])
				return false;
		}
		return true;
	}

	TemplateDecl
	bool operator!=(const Point1 & lhs, const point2 & rhs) noexcept {
		return !(lhs == rhs);
	}

#define OperatorImpl(name, assign_name, op) \
	TemplateDecl \
	Point1 name(const Point1 & lhs, const point2 & rhs) noexcept { \
		Point1 ret = lhs; \
		ret op rhs; \
		return ret; \
	} \
	TemplateDecl \
	Point1 & assign_name(Point1 & lhs, const point2 & rhs) noexcept { \
		for (size_t i = 0; i < D; ++i) \
			lhs.raw[i] op (P1)rhs.raw[i]; \
		return lhs; \
	}

	OperatorImpl(operator+, operator+=, +=)
	OperatorImpl(operator-, operator-=, -=)
	OperatorImpl(operator*, operator*=, *=)
	OperatorImpl(operator/, operator/=, /=)
#undef OperatorImpl

	TemplateDecl
	P1 dot(const Point1 & lhs, const point2 & rhs) noexcept {
		P1 ret = 0;
		for (size_t i = 0; i < D; ++i)
			ret += lhs.raw[i] * rhs.raw[i];
		return ret;
	}

#undef TemplateDecl
#undef Point1
#undef point2

#define TemplateDecl template<typename P, size_t D>
#define TPoint point<P, D>

	TemplateDecl
		TPoint
		operator-(const TPoint & p) noexcept {
		TPoint ret = p;
		for (size_t i = 0; i < D; ++i)
			ret[i] = -ret[i];
		return ret;
	}

#define OperatorImpl(name, assign_name, op) \
	TemplateDecl \
	TPoint name(const TPoint & lhs, float rhs) noexcept { \
		TPoint ret = lhs; \
		ret op rhs; \
		return ret; \
	} \
	TemplateDecl \
	TPoint & assign_name(TPoint & lhs, float rhs) noexcept { \
		for (size_t i = 0; i < D; ++i) \
			lhs.raw[i] op(P) rhs; \
		return lhs; \
	}

	OperatorImpl(operator*, operator*=, *=);
	OperatorImpl(operator/, operator/=, /=);

#undef OperatorImpl

	TemplateDecl
	TPoint operator*(float lhs, const TPoint & rhs) {
		return rhs * lhs;
	}

	TemplateDecl
	P get_length_squared(const TPoint & p) noexcept {
		P ret = 0;
		for (size_t i = 0; i < D; ++i)
			ret += p.raw[i] * p.raw[i];
		return ret;
	}

	TemplateDecl
	float get_length(const TPoint & p) noexcept {
		return sqrtf((float)get_length_squared(p));
	}

	TemplateDecl
	void normalize(TPoint & p, float desired_length) noexcept {
		auto n = get_length(p);
		if (n > 0.0f) {
			n = desired_length / n;
			for (size_t i = 0; i < D; ++i)
				p.raw[i] *= (P)n;
		}
	}

	TemplateDecl
	point<float, D> normalized(const TPoint & p, float desired_length) noexcept {
		point<float, D> ret = p;
		normalize(ret, desired_length);
		return ret;
	}

	// clang-format on
#undef TemplateDecl
#undef TPoint

	template<typename Precision>
	vec<Precision, 3> cross(const vec<Precision, 3> & v, const vec<Precision, 3> & v2) noexcept {
		return {
			v.y * v2.z - v.z * v2.y,
			v.z * v2.x - v.x * v2.z,
			v.x * v2.y - v.y * v2.x
		};
	}
}