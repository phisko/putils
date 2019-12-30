# [Point<Precision, Dimensions>, Rect](Point.hpp)

Abstraction to a 2D or 3D point/rectangle.

## Point

### Members

#### Coordinates

```cpp
Precision x;
Precision y;
{ Precision z; } // If Dimensions == 3
```

#### Operators

```cpp
bool operator==(const Point<P> &rhs) const;
bool operator!=(const Point<P> &rhs) const;

Point operator-() const noexcept;

Point operator+(const Point<P> &rhs) const;
Point &operator+=(const Point<P> &rhs);

Point operator-(const Point<P> &rhs) const;
Point &operator-=(const Point<P> &rhs);

Point operator*(const Point & rhs) const noexcept;
Point & operator*=(const Point & rhs) noexcept;

Point operator*(float rhs) const noexcept;
Point & operator*=(float rhs) noexcept;

Point operator/(const Point & rhs) const noexcept;
Point & operator/=(const Point & rhs) noexcept;

Point operator/(float rhs) const noexcept;
Point & operator/=(float rhs) noexcept;
```

#### getDistanceTo

```cpp
float getDistanceTo(const Point & rhs) const noexcept;
```
Returns the distance to another point.

#### getLengthSquared, getLength

```cpp
float getLengthSquared() const noexcept;
float getLength() const noexcept;
```

Return the (optionnally squared, to avoid call to `sqrt`) length of the vector.

#### getAngleTo, getYawTo, getPitchTo

```cpp
float getAngleTo(const Point & rhs) const noexcept; // If Dimensions == 2
float getYawTo(const Point & rhs) const noexcept; // If Dimensions == 3
float getPitchTo(const Point & rhs) const noexcept; // If Dimensions == 3
```

Return the angle (or pitch/yaw) between this point and another.

#### normalize

```cpp
void normalize(float desiredLength = 1);
```

Normalizes the vector so that its total length matches the desired value.

## Rect

### Members

#### Coordinates

```cpp
Point<Precision> position;
Point<Precision> size;
```

#### Operators

```cpp
bool operator==(const Rect<P> &rhs) const;
bool operator!=(const Rect<P> &rhs) const;
```