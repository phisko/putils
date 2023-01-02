#pragma once

// reflection
#include "putils/reflection.hpp"

// putils
#include "hash.hpp"
#include "lengthof.hpp"

namespace putils {
	template<typename Precision, size_t Dimensions>
	struct point {
		using buffer = Precision[Dimensions];
		buffer raw;
	};

	template<typename Precision, size_t Dimensions>
	using vec = point<Precision, Dimensions>;

	using point3d = point<double, 3>;
	using vec3d = point3d;

	using point2d = point<double, 2>;
	using vec2d = point3d;

	using point3i = point<int, 3>;
	using vec3i = point3i;

	using point2i = point<int, 2>;
	using vec2i = point2i;

	using point3ui = point<unsigned int, 3>;
	using vec3ui = point3ui;

	using point2ui = point<unsigned int, 2>;
	using vec2ui = point2ui;

	using point3f = point<float, 3>;
	using vec3f = point3f;

	using point2f = point<float, 2>;
	using vec2f = point2f;

	template<typename P, size_t D>
#define refltype point<P, D>
	putils_reflection_info_template {
		static constexpr const char class_name_priv[] = { 'p', 'o', 'i', 'n', 't', '0' + D, 0 };
		static constexpr const char * class_name = class_name_priv;
		putils_reflection_attributes(
			putils_reflection_attribute(raw)
		);
	};
#undef refltype

	template<typename Precision>
	struct point<Precision, 2> {
		using buffer = Precision[2];
		union {
			buffer raw;
			struct {
				Precision x;
				Precision y;
			};
		};

		point() noexcept = default;
		point(Precision x, Precision y) noexcept;
		point(const Precision * p) noexcept;

		point(const point &) noexcept = default;
		point & operator=(const point &) noexcept = default;

		template<typename P>
		point(const point<P, 2> & other) noexcept;
		template<typename P>
		point & operator=(const point<P, 2> & other) noexcept;

		template<typename P>
		point(const point<P, 3> & other) noexcept;
		template<typename P>
		point & operator=(const point<P, 3> & other) noexcept;

		Precision & operator[](size_t index) noexcept;
		Precision operator[](size_t index) const noexcept;
	};

	template<typename T>
#define refltype point<T, 2>
	putils_reflection_info_template {
		putils_reflection_custom_class_name(point2);
		putils_reflection_attributes(
			putils_reflection_attribute(raw),
			putils_reflection_attribute(x),
			putils_reflection_attribute(y)
		);
	};
#undef refltype

	template<typename Precision>
	struct point<Precision, 3> {
		using buffer = Precision[3];
		union {
			buffer raw;
			struct {
				Precision x;
				Precision y;
				Precision z;
			};
		};

		point() noexcept = default;
		point(Precision x, Precision y, Precision z) noexcept;
		point(const Precision * p) noexcept;

		point(const point &) noexcept = default;
		point & operator=(const point &) noexcept = default;

		template<typename P>
		point(const point<P, 2> & other) noexcept;
		template<typename P>
		point & operator=(const point<P, 2> & other) noexcept;

		template<typename P>
		point(const point<P, 3> & other) noexcept;
		template<typename P>
		point & operator=(const point<P, 3> & other) noexcept;

		Precision & operator[](size_t index) noexcept;
		Precision operator[](size_t index) const noexcept;
	};

	template<typename T>
#define refltype point<T, 3>
	putils_reflection_info_template {
		putils_reflection_custom_class_name(point3);
		putils_reflection_attributes(
			putils_reflection_attribute(x),
			putils_reflection_attribute(y),
			putils_reflection_attribute(z)
		);
	};
#undef refltype

#define TemplateDecl template<typename P1, typename P2, size_t D>
#define Point1 point<P1, D>
#define point2 point<P2, D>
	// clang-format off

	TemplateDecl
	bool operator==(const Point1 & lhs, const point2 & rhs) noexcept;
	TemplateDecl
	bool operator!=(const Point1 & lhs, const point2 & rhs) noexcept;

	TemplateDecl
	Point1 operator+(const Point1 & lhs, const point2 & rhs) noexcept;
	TemplateDecl
	Point1 & operator+=(Point1 & lhs, const point2 & rhs) noexcept;

	TemplateDecl
	Point1 operator-(const Point1 & lhs, const point2 & rhs) noexcept;
	TemplateDecl
	Point1 & operator-=(Point1 & lhs, const point2 & rhs) noexcept;

	TemplateDecl
	Point1 operator*(const Point1 & lhs, const point2 & rhs) noexcept;
	TemplateDecl
	Point1 & operator*=(Point1 & lhs, const point2 & rhs) noexcept;

	TemplateDecl
	Point1 operator/(const Point1 & lhs, const point2 & rhs) noexcept;
	TemplateDecl
	Point1 & operator/=(Point1 & lhs, const point2 & rhs) noexcept;

	TemplateDecl
	P1 dot(const Point1 & lhs, const point2 & rhs) noexcept;

#undef TemplateDecl
#undef Point1
#undef point2

#define TemplateDecl template<typename P, size_t D>
#define TPoint point<P, D>

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
	P get_length_squared(const TPoint & p) noexcept;
	TemplateDecl
	float get_length(const TPoint & p) noexcept;

	TemplateDecl
	void normalize(TPoint & p, float desired_length = 1) noexcept;

	TemplateDecl
	point<float, D> normalized(const TPoint & p, float desired_length = 1) noexcept;

	// clang-format on
#undef TemplateDecl
#undef TPoint

	template<typename Precision>
	vec<Precision, 3> cross(const vec<Precision, 3> & v, const vec<Precision, 3> & v2) noexcept;
}

#include "point.inl"
