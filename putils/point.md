# [point<Precision, Dimensions>](point.hpp)

Abstraction to a 2D or 3D point.

## Aliases

```cpp
template<typename Precision, size_t Dimensions>
using vec = point<Precision, Dimensions>;

using point3d = point<double, 3>;
using vec3d = point3d;

using point3d = point<double, 2>;
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

### get_length_squared, get_length

```cpp
Precision get_length_squared(const point & p) const noexcept;
float get_length(const point & p) const noexcept;
```

Return the (optionnally squared, to avoid call to `sqrt`) length of the vector.

### normalize, normalized

```cpp
void normalize(point & p, float desired_length = 1);
point normalized(const point & p, float desired_length = 1) noexcept;
```

Normalizes the vector so that its total length matches the desired value.

### cross

```cpp
point cross(const point & v, const point & v2) noexcept;
```

Performs a cross product on two vectors.