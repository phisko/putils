#include "Point.hpp"

#include <cmath>

namespace putils {
#define TemplateDecl template<typename Precision>
#define TPoint Point<Precision, 2>
	TemplateDecl
	TPoint::Point(Precision x, Precision y) noexcept
		: x(x), y(y)
	{}

	TemplateDecl
	TPoint::Point(const Precision * p) noexcept
		: x(p[0]), y(p[1])
	{}

	TemplateDecl
	template<typename P>
	TPoint::Point(const Point<P, 2> & other) noexcept
		: x((Precision)other.x), y((Precision)other.y)
	{}

	TemplateDecl
	template<typename P>
	TPoint & TPoint::operator=(const Point<P, 2> & other) noexcept {
		x = (Precision)other.x;
		y = (Precision)other.y;
		return *this;
	}

	TemplateDecl
	template<typename P>
	TPoint::Point(const Point<P, 3> & other) noexcept
		: x((Precision)other.x), y((Precision)other.y)
	{}

	TemplateDecl
	template<typename P>
	TPoint & TPoint::operator=(const Point<P, 3> & other) noexcept {
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

#define TPoint Point<Precision, 3>
	TemplateDecl
	TPoint::Point(Precision x, Precision y, Precision z) noexcept
		: x(x), y(y), z(z)
	{}

	TemplateDecl
	TPoint::Point(const Precision * p) noexcept
		: x(p[0]), y(p[1]), z(p[2])
	{}

	TemplateDecl
	template<typename P>
	TPoint::Point(const Point<P, 2> & other) noexcept
		: x((Precision)other.x), y((Precision)other.y), z(0)
	{}

	TemplateDecl
	template<typename P>
	TPoint & TPoint::operator=(const Point<P, 2> & other) noexcept {
		x = (Precision)other.x; y = (Precision)other.y; z = 0;
		return *this;
	}

	TemplateDecl
	template<typename P>
	TPoint::Point(const Point<P, 3> & other) noexcept
		: x((Precision)other.x), y((Precision)other.y), z((Precision)other.z)
	{}

	TemplateDecl
	template<typename P>
	TPoint & TPoint::operator=(const Point<P, 3> & other) noexcept {
		x = (Precision)other.x; y = (Precision)other.y; z = (Precision)other.z;
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
#define Point1 Point<P1, D>
#define Point2 Point<P2, D>

	TemplateDecl
	bool operator==(const Point1 & lhs, const Point2 & rhs) noexcept {
        for (size_t i = 0; i < D; ++i) {
            if (lhs[i] != rhs[i])
                return false;
        }
        return true;
	}

	TemplateDecl
	bool operator!=(const Point1 & lhs, const Point2 & rhs) noexcept {
		return !(lhs == rhs);
	}

#define OperatorImpl(name, assignName, op) \
	TemplateDecl\
	Point1 name(const Point1 & lhs, const Point2 & rhs) noexcept {\
		Point1 ret = lhs;\
		ret op rhs;\
		return ret;\
	}\
	TemplateDecl\
	Point1 & assignName(Point1 & lhs, const Point2 & rhs) noexcept {\
		for (size_t i = 0; i < D; ++i)\
			lhs.raw[i] op (P1)rhs.raw[i];\
		return lhs;\
	}

	OperatorImpl(operator+, operator+=, +=)
	OperatorImpl(operator-, operator-=, -=)
	OperatorImpl(operator*, operator*=, *=)
	OperatorImpl(operator/, operator/=, /=)

#undef OperatorImpl

	TemplateDecl
	P1 dot(const Point1 & lhs, const Point2 & rhs) noexcept {
		P1 ret = 0;
		for (size_t i = 0; i < D; ++i)
			ret += lhs.raw[i] * rhs.raw[i];
		return ret;
	}

#undef TemplateDecl
#undef Point1
#undef Point2

#define TemplateDecl template<typename P, size_t D>
#define TPoint Point<P, D>

	TemplateDecl
	TPoint operator-(const TPoint & p) noexcept {
		TPoint ret = p;
		for (size_t i = 0; i < D; ++i)
			ret[i] = -ret[i];
		return ret;
	}

#define OperatorImpl(name, assignName, op) \
	TemplateDecl\
	TPoint name(const TPoint & lhs, float rhs) noexcept {\
		TPoint ret = lhs;\
		ret op rhs;\
		return ret;\
	}\
	TemplateDecl\
	TPoint & assignName(TPoint & lhs, float rhs) noexcept {\
		for (size_t i = 0; i < D; ++i)\
			lhs.raw[i] op rhs;\
		return lhs;\
	}\

	OperatorImpl(operator*, operator*=, *=);
	OperatorImpl(operator/, operator/=, /=);

#undef OperatorImpl

	TemplateDecl
	TPoint operator*(float lhs, const TPoint & rhs) {
		return rhs * lhs;
	}

	TemplateDecl
	P getLengthSquared(const TPoint & p) noexcept {
		P ret = 0;
		for (size_t i = 0; i < D; ++i)
			ret += p.raw[i] * p.raw[i];
		return ret;
	}

	TemplateDecl
	float getLength(const TPoint & p) noexcept {
		return sqrtf((float)getLengthSquared(p));
	}

	TemplateDecl
	void normalize(TPoint & p, float desiredLength) noexcept {
		auto n = getLength(p);
		if (n > 0.0f) {
			n = desiredLength / n;
			for (size_t i = 0; i < D; ++i)
				p.raw[i] *= (P)n;
		}
	}

	TemplateDecl
	Point<float, D> normalized(const TPoint & p, float desiredLength) noexcept {
		Point<float, D> ret = p;
		normalize(ret, desiredLength);
		return ret;
	}

#undef TemplateDecl
#undef TPoint

	template<typename Precision>
	Vector<Precision, 3> cross(const Vector<Precision, 3> & v, const Vector<Precision, 3> &v2) noexcept {
		return {
			v.y * v2.z - v.z * v2.y ,
			v.z * v2.x - v.x * v2.z ,
			v.x * v2.y - v.y * v2.x
		};
	}
}