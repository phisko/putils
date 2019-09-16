#pragma once

#include "hash.hpp"
#include "reflection/Reflectible.hpp"

namespace putils {
    template<typename Precision, std::size_t Dimensions = 2>
    struct Point : putils::Reflectible<Point<Precision, Dimensions>> {
		union {
			Precision raw[2];
			struct {
				Precision x;
				Precision y;
			};
		};

        Point(Precision x = 0, Precision y = 0)
                : x(x), y(y) {}

        Point(const Point &) noexcept = default;
        Point & operator=(const Point &) noexcept = default;

        Point(Point &&) noexcept = default;
        Point & operator=(Point &&) noexcept = default;

        template<typename P>
        Point(const Point<P, 3> & other) : x(other.x), y(other.y) {}

        template<typename P>
        bool operator==(const Point<P> & rhs) const noexcept { return x == rhs.x && y == rhs.y; }

        template<typename P>
        bool operator!=(const Point<P> & rhs) const noexcept { return !(*this == rhs); }

        template<typename P>
        Point operator+(const Point<P> & rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }

        template<typename P>
        Point & operator+=(const Point<P> & rhs) noexcept {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        template<typename P>
        Point operator-(const Point<P> & rhs) const noexcept { return { x - rhs.x, y - rhs.y }; }

        template<typename P>
        Point & operator-=(const Point<P> & rhs) noexcept {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

		Point operator*(const Point & rhs) const noexcept { return { x * rhs.x, y * rhs.y }; }
		Point & operator*=(const Point & rhs) const noexcept { x *= rhs.x; y *= rhs.y; return *this; }

		Point operator*(float rhs) const noexcept { return { x * rhs, y * rhs }; }
		Point & operator*=(float rhs) const noexcept { x *= rhs; y *= rhs; return *this; }

		Point operator/(const Point & rhs) const noexcept { return { x / rhs.x, y / rhs.y }; }
		Point & operator/=(const Point & rhs) const noexcept { x /= rhs.x; y /= rhs.y; return *this; }

		Point operator/(float rhs) const noexcept { return { x / rhs, y / rhs }; }
		Point & operator/=(float rhs) const noexcept { x /= rhs; y /= rhs; return *this; }

        template<typename P>
        float distanceTo(const Point<P> & rhs) const noexcept {
            return std::sqrtf(
                    std::powf((float)x - (float)rhs.x, 2) +
                    std::powf((float)y - (float)rhs.y, 2)
            );
        }

        float getLength() const noexcept {
            return std::sqrtf(
                    std::powf((float)x, 2.f) +
                    std::powf((float)y, 2.f)
            );
        }

        float angleTo(const Point<Precision, 3> & rhs) const noexcept {
            return std::atan2f((float)y - (float)rhs.y, (float)rhs.x - (float)x);
        }

		void normalize(float desiredLength = 1) {
			auto n = getLength();
			if (n > 0.0f) {
				n = desiredLength / n;
				x *= (Precision)n;
				y *= (Precision)n;
			}
		}

        pmeta_get_class_name("Point2");
        pmeta_get_attributes(
                pmeta_reflectible_attribute(&Point::x),
                pmeta_reflectible_attribute(&Point::y)
        );
        pmeta_get_methods(
                std::string_view(pmeta_nameof(distanceTo)), &Point::distanceTo<Precision>,
                pmeta_reflectible_attribute(&Point::getLength),
                pmeta_reflectible_attribute(&Point::angleTo),
                pmeta_reflectible_attribute(&Point::normalize)
        );
        pmeta_get_parents();
    };

    template<typename Precision, std::size_t Dimensions = 2>
    struct Rect : public Reflectible<Rect<Precision, Dimensions>> {
        Point<Precision> position;
        Point<Precision> size;

        Rect(Point<Precision, Dimensions> position = {}, Point<Precision, Dimensions> size = {})
                : position(position), size(size) {}

        template<typename P>
        bool operator==(const Rect<P> & rhs) const { return position == rhs.position && size == rhs.size; }

        template<typename P>
        bool operator!=(const Rect<P> & rhs) const { return !(*this == rhs); }

        bool intersect(const Rect & other, bool inclusiveBorders = false) const {
            if (inclusiveBorders)
                return !(position.x > other.position.x + other.size.x ||
                         position.x + size.x < other.position.x ||
                         position.y > other.position.y + other.size.y ||
                         position.y + size.y < other.position.y
                );

            return !(position.x >= other.position.x + other.size.x ||
                     position.x + size.x <= other.position.x ||
                     position.y >= other.position.y + other.size.y ||
                     position.y + size.y <= other.position.y
            );
        }

		Point<Precision, Dimensions> getCenter() const {
			return { position.x + size.x / 2, position.y - size.y / 2 };
        }

		void setCenter(const Point<Precision, Dimensions> & center) {
			position.x = center.x - size.x / 2.0;
			position.y = center.y + size.y / 2.0;
		}

        bool contains(const Point<Precision, 2> & point) const {
            return (position.x <= point.x &&
                    position.x + size.x > point.x &&
                    position.y <= point.y &&
                    position.y + size.y > point.y
            );
        }

        pmeta_get_class_name("Rect2");
        pmeta_get_attributes(
                pmeta_reflectible_attribute(&Rect::position),
                pmeta_reflectible_attribute(&Rect::size)
        );
        pmeta_get_methods(
                pmeta_reflectible_attribute(&Rect::intersect),
                pmeta_reflectible_attribute(&Rect::contains),
                pmeta_reflectible_attribute(&Rect::getCenter),
                pmeta_reflectible_attribute(&Rect::setCenter)
        );
        pmeta_get_parents();
    };

    template<typename Precision>
    struct Point<Precision, 3> : public Reflectible<Point<Precision, 3>> {
		union {
			Precision raw[3];
			struct {
				Precision x;
				Precision y;
				Precision z;
			};
		};

        Point(Precision x = 0, Precision y = 0, Precision z = 0)
                : x(x), y(y), z(z) {}

        Point(const Point &) noexcept = default;
        Point & operator=(const Point &) noexcept = default;

        Point(Point &&) noexcept = default;
        Point & operator=(Point &&) noexcept = default;

        template<typename P>
        Point(const Point<P, 2> & other) : x(other.x), y(other.y), z(0) {}

        template<typename P>
        bool operator==(const Point<P, 3> & rhs) const noexcept { return x == rhs.x && y == rhs.y && z == rhs.z; }

        template<typename P>
        bool operator!=(const Point<P, 3> & rhs) const noexcept { return !(*this == rhs); }

        Point operator-() const noexcept { return { -x, -y, -z }; }

        template<typename P>
        Point operator+(const Point<P, 3> & rhs) const noexcept { return { x + rhs.x, y + rhs.y, z + rhs.z }; }

        template<typename P>
        Point & operator+=(const Point<P, 3> & rhs) noexcept {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        template<typename P>
        Point operator-(const Point<P, 3> & rhs) const noexcept { return { x - rhs.x, y - rhs.y, z - rhs.z }; }

        template<typename P>
        Point & operator-=(const Point<P, 3> & rhs) noexcept {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

		Point operator*(const Point & rhs) const noexcept { return { x * rhs.x, y * rhs.y, z * rhs.z }; }
		Point & operator*=(const Point & rhs) noexcept { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }

		Point operator*(float rhs) const noexcept { return { x * rhs, y * rhs, z * rhs }; }
		Point & operator*=(float rhs) noexcept { x *= rhs; y *= rhs; z *= rhs; return *this; }

		Point operator/(const Point & rhs) const noexcept { return { x / rhs.x, y / rhs.y, z / rhs.z }; }
		Point & operator/=(const Point & rhs) noexcept { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }

		Point operator/(float rhs) const noexcept { return { x / rhs, y / rhs, z / rhs }; }
		Point & operator/=(float rhs) noexcept { x /= rhs; y /= rhs; z /= rhs; return *this; }

        template<typename P>
        float distanceTo(const Point<P, 3> & rhs) const noexcept {
            return std::sqrtf(
                    std::powf((float)x - (float)rhs.x, 2.f) +
                    std::powf((float)y - (float)rhs.y, 2.f) +
                    std::powf((float)z - (float)rhs.z, 2.f)
            );
        }

        float getLength() const noexcept {
            return std::sqrtf(
                    std::powf((float)x, 2.f) +
                    std::powf((float)y, 2.f) +
                    std::powf((float)z, 2.f)
            );
        }

        float angleToXY(const Point<Precision, 3> & rhs) const noexcept {
            return std::atan2f((float)y - (float)rhs.y, (float)rhs.x - (float)x);
        }

        float angleToXZ(const Point<Precision, 3> & rhs) const noexcept {
            return std::atan2f((float)z - (float)rhs.z, (float)rhs.x - (float)x);
        }

		void normalize(float desiredLength = 1) {
			auto n = getLength();
			if (n > 0.0f) {
				n = desiredLength / n;
				x *= (Precision)n;
				y *= (Precision)n;
				z *= (Precision)n;
			}
		}

        pmeta_get_class_name("Point3");
        pmeta_get_attributes(
                pmeta_reflectible_attribute(&Point::x),
                pmeta_reflectible_attribute(&Point::y),
                pmeta_reflectible_attribute(&Point::z)
        );
        pmeta_get_methods(
                std::string_view(pmeta_nameof(distanceTo)), &Point::distanceTo<Precision>,
                pmeta_reflectible_attribute(&Point::getLength),
                pmeta_reflectible_attribute(&Point::angleToXY),
                pmeta_reflectible_attribute(&Point::angleToXZ),
                pmeta_reflectible_attribute(&Point::normalize)
        );
        pmeta_get_parents();
    };

    template<typename Precision>
    struct Rect<Precision, 3> : public Reflectible<Rect<Precision, 3>> {
        Point<Precision, 3> position;
        Point<Precision, 3> size;

        Rect(Point<Precision, 3> position = {}, Point<Precision, 3> size = {})
                : position(position), size(size) {}

        template<typename P>
        bool operator==(const Rect<P> & rhs) { return position == rhs.position && size == rhs.size; }

        template<typename P>
        bool operator!=(const Rect<P> & rhs) { return !(*this == rhs); }

        bool intersect(const Rect & other, bool inclusiveBorders = false) const {
            if (inclusiveBorders)
                return !(position.x > other.position.x + other.size.x ||
                         position.x + size.x < other.position.x ||
                         position.y > other.position.y + other.size.y ||
                         position.y + size.y < other.position.y ||
                         position.z > other.position.z + other.size.z ||
                         position.z + size.z < other.position.z
                );

            return !(position.x >= other.position.x + other.size.x ||
                     position.x + size.x <= other.position.x ||
                     position.y >= other.position.y + other.size.y ||
                     position.y + size.y <= other.position.y ||
                     position.z >= other.position.z + other.size.z ||
                     position.z + size.z <= other.position.z
            );
        }

        bool contains(const Point<Precision, 3> & point) const {
            return (position.x <= point.x &&
                    position.x + size.x > point.x &&
                    position.y <= point.y &&
                    position.y + size.y > point.y &&
                    position.z <= point.z &&
                    position.z + size.z > point.z
            );
        }

		Point<Precision, 3> getCenter() const {
			return { position.x + size.x / 2, position.y - size.y / 2, position.z - size.z / 2 };
        }

		void setCenter(const Point<Precision, 3> & center) {
			position.x = center.x - size.x / 2;
			position.y = center.y + size.y / 2;
			position.z = center.z + size.z / 2;
        }

        pmeta_get_class_name("Rect3");
        pmeta_get_attributes(
                pmeta_reflectible_attribute(&Rect::position),
                pmeta_reflectible_attribute(&Rect::size)
        );
        pmeta_get_methods(
                pmeta_reflectible_attribute(&Rect::intersect),
                pmeta_reflectible_attribute(&Rect::contains),
                pmeta_reflectible_attribute(&Rect::getCenter),
                pmeta_reflectible_attribute(&Rect::setCenter)
        );
        pmeta_get_parents();
    };

	template<typename Precision, size_t Dimensions>
	using Vector = Point<Precision, Dimensions>;

    using Point3d = Point<double, 3>;
	using Vector3d = Point3d;
    using Rect3d = Rect<double, 3>;

    using Point2d = Point<double, 2>;
	using Vector2d = Point2d;
    using Rect2d = Rect<double, 2>;

    using Point3i = Point<int, 3>;
	using Vector3i = Point3i;
    using Rect3i = Rect<int, 3>;

    using Point2i = Point<int, 2>;
	using Vector2i = Point2i;
    using Rect2i = Rect<int, 2>;

    using Point3ui = Point<unsigned int, 3>;
	using Vector3ui = Point3ui;
    using Rect3ui = Rect<unsigned int, 3>;

    using Point2ui = Point<unsigned int, 2>;
	using Vector2ui = Point2ui;
    using Rect2ui = Rect<unsigned int, 2>;

    using Point3f = Point<float, 3>;
	using Vector3f = Point3f;
    using Rect3f = Rect<float, 3>;

    using Point2f = Point<float, 2>;
	using Vector2f = Point2f;
    using Rect2f = Rect<float, 2>;

	template<typename Precision>
	inline putils::Vector<Precision, 3> cross(const Vector<Precision, 3> & v, const Vector<Precision, 3> &v2) {
		return {
			v.y * v2.z - v.z * v2.y ,
			v.z * v2.x - v.x * v2.z ,
			v.x * v2.y - v.y * v2.x
		};
	}

	template<typename Precision>
	inline Precision dot(const Vector<Precision, 3> &v, const Vector<Precision, 3> &v2) {
		return v.x * v2.x + v.y * v2.y + v.z * v2.z;
	}
}

namespace std {
    template<typename Precision>
    struct hash<putils::Point<Precision>> {
        size_t operator()(const putils::Point<Precision> & coord) const noexcept {
            return putils::PairHash().operator()(std::make_pair(coord.x, coord.y));
        }
    };
}

namespace putils {
    template<typename Precision>
    using PointHash = std::hash<putils::Point<Precision>>;
}

