# [Point<Precision, Dimensions>](Point.hpp)

Abstraction to a 2D or 3D point.

## Aliases

```cpp
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
```

## Members

### Coordinates

```cpp
union {
    Precision raw[Dimensions];
    struct {
        Precision x;
        Precision y;
        { Precision z; } // If Dimensions == 3
    };
};
```

### Ctors and assignment operators

```cpp
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
```

### Access

```cpp
Precision & operator[](size_t index) noexcept;
Precision operator[](size_t index) const noexcept;

operator Precision[Dimensions] & () noexcept;
operator const Precision[Dimensions] & () const noexcept;
```

## Helper functions

In addition to these functions, operators are overloaded to allow standard vector operations (comparison, addition, substraction, multiplication, division).

### getLengthSquared, getLength

```cpp
Precision getLengthSquared(const Point & p) const noexcept;
float getLength(const Point & p) const noexcept;
```

Return the (optionnally squared, to avoid call to `sqrt`) length of the vector.

### normalize, normalized

```cpp
void normalize(Point & p, float desiredLength = 1);
Point normalized(const Point & p, float desiredLength = 1) noexcept;
```

Normalizes the vector so that its total length matches the desired value.

### cross

```cpp
Point cross(const Point & v, const Point & v2) noexcept;
```

Performs a cross product on two vectors.