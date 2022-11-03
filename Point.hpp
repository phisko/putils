#pragma once

// reflection
#include "reflection.hpp"

// putils
#include "hash.hpp"
#include "lengthof.hpp"

namespace putils {
    template<typename Precision, size_t Dimensions>
	struct Point {
		using Buffer = Precision[Dimensions];
		Buffer raw;

		operator Buffer & () noexcept { return raw; }
		operator const Buffer & () const noexcept { return raw; }
	};

	template<typename Precision, size_t Dimensions>
	using Vector = Point<Precision, Dimensions>;

    using Point3d = Point<double, 3>;
	using Vector3d = Point3d;

    using Point2d = Point<double, 2>;
	using Vector2d = Point2d;

    using Point3i = Point<int, 3>;
	using Vector3i = Point3i;

    using Point2i = Point<int, 2>;
	using Vector2i = Point2i;

    using Point3ui = Point<unsigned int, 3>;
	using Vector3ui = Point3ui;

    using Point2ui = Point<unsigned int, 2>;
	using Vector2ui = Point2ui;

    using Point3f = Point<float, 3>;
	using Vector3f = Point3f;

    using Point2f = Point<float, 2>;
	using Vector2f = Point2f;


	template<typename P, size_t D>
#define refltype Point<P, D>
	putils_reflection_info_template{
		static constexpr const char class_name_priv[] = { 'P', 'o', 'i', 'n', 't', '0' + D, 0 };
        static constexpr const char * class_name = class_name_priv;
		putils_reflection_attributes(
			putils_reflection_attribute(raw)
		);
	};
#undef refltype

    template<typename Precision>
    struct Point<Precision, 2> {
		using Buffer = Precision[2];
		union {
			Buffer raw;
			struct {
				Precision x;
				Precision y;
			};
		};

		Point() noexcept = default;
		Point(Precision x, Precision y) noexcept;
		Point(const Precision * p) noexcept;

        Point(const Point &) noexcept = default;
        Point & operator=(const Point &) noexcept = default;

        template<typename P>
		Point(const Point<P, 2> & other) noexcept;
        template<typename P>
		Point & operator=(const Point<P, 2> & other) noexcept;

        template<typename P>
		Point(const Point<P, 3> & other) noexcept;
        template<typename P>
		Point & operator=(const Point<P, 3> & other) noexcept;

		Precision & operator[](size_t index) noexcept;
		Precision operator[](size_t index) const noexcept;

		operator Buffer & () noexcept { return raw; }
		operator const Buffer & () const noexcept { return raw; }
	};

	template<typename T>
#define refltype Point<T, 2>
	putils_reflection_info_template{
		putils_reflection_custom_class_name(Point2);
		putils_reflection_attributes(
			putils_reflection_attribute(raw),
			putils_reflection_attribute(x),
			putils_reflection_attribute(y)
		);
	};
#undef refltype

    template<typename Precision>
    struct Point<Precision, 3> {
		using Buffer = Precision[3];
		union {
			Buffer raw;
			struct {
				Precision x;
				Precision y;
				Precision z;
			};
		};

		Point() noexcept = default;
		Point(Precision x, Precision y, Precision z) noexcept;
		Point(const Precision * p) noexcept;

        Point(const Point &) noexcept = default;
        Point & operator=(const Point &) noexcept = default;

		template<typename P>
		Point(const Point<P, 2> & other) noexcept;
        template<typename P>
		Point & operator=(const Point<P, 2> & other) noexcept;

        template<typename P>
		Point(const Point<P, 3> & other) noexcept;
		template<typename P>
		Point & operator=(const Point<P, 3> & other) noexcept;

		Precision & operator[](size_t index) noexcept;
		Precision operator[](size_t index) const noexcept;

		operator Buffer & () noexcept { return raw; }
		operator const Buffer & () const noexcept { return raw; }
	};

	template<typename T>
#define refltype Point<T, 3>
	putils_reflection_info_template{
		putils_reflection_custom_class_name(Point3);
		putils_reflection_attributes(
			putils_reflection_attribute(x),
			putils_reflection_attribute(y),
			putils_reflection_attribute(z)
		);
	};
#undef refltype


#define TemplateDecl template<typename P1, typename P2, size_t D>
#define Point1 Point<P1, D>
#define Point2 Point<P2, D>

	TemplateDecl
	bool operator==(const Point1 & lhs, const Point2 & rhs) noexcept;
	TemplateDecl
	bool operator!=(const Point1 & lhs, const Point2 & rhs) noexcept;

	TemplateDecl
	Point1 operator+(const Point1 & lhs, const Point2 & rhs) noexcept;
	TemplateDecl
	Point1 & operator+=(Point1 & lhs, const Point2 & rhs) noexcept;

	TemplateDecl
	Point1 operator-(const Point1 & lhs, const Point2 & rhs) noexcept;
	TemplateDecl
	Point1 & operator-=(Point1 & lhs, const Point2 & rhs) noexcept;

	TemplateDecl
	Point1 operator*(const Point1 & lhs, const Point2 & rhs) noexcept;
	TemplateDecl
	Point1 & operator*=(Point1 & lhs, const Point2 & rhs) noexcept;

	TemplateDecl
	Point1 operator/(const Point1 & lhs, const Point2 & rhs) noexcept;
	TemplateDecl
	Point1 & operator/=(Point1 & lhs, const Point2 & rhs) noexcept;

	TemplateDecl
	P1 dot(const Point1 & lhs, const Point2 & rhs) noexcept;

#undef TemplateDecl
#undef Point1
#undef Point2

#define TemplateDecl template<typename P, size_t D>
#define TPoint Point<P, D>

	TemplateDecl
	TPoint operator-(const TPoint & p) noexcept;

	TemplateDecl
	TPoint operator*(const TPoint & lhs, float rhs) noexcept;
	TemplateDecl
	TPoint & operator*=(TPoint & lhs, float rhs) noexcept;

	TemplateDecl
	TPoint operator*(float lhs, const TPoint & rhs);

	TemplateDecl
	TPoint operator/(const TPoint & lhs, float rhs) noexcept;
	TemplateDecl
	TPoint & operator/=(TPoint & lhs, float rhs) noexcept;

	TemplateDecl
	P getLengthSquared(const TPoint & p) noexcept;
	TemplateDecl
	float getLength(const TPoint & p) noexcept;

	TemplateDecl
	void normalize(TPoint & p, float desiredLength = 1) noexcept;

	TemplateDecl
	Point<float, D> normalized(const TPoint & p, float desiredLength = 1) noexcept;

#undef TemplateDecl
#undef TPoint

	template<typename Precision>
	Vector<Precision, 3> cross(const Vector<Precision, 3> & v, const Vector<Precision, 3> & v2) noexcept;
}

#include "Point.inl"
