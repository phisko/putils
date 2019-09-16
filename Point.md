# [Point<Precision, Dimensions>, Rect](Point.hpp)

Abstraction to a 2D or 3D point/rectangle.

## Point

### Members

##### Coordinates

```cpp
Precision x;
Precision y;
{ Precision z; } // If Dimensions == 3
```

##### Operators

```cpp
bool operator==(const Point<P> &rhs) const;
bool operator!=(const Point<P> &rhs) const;
Point operator+(const Point<P> &rhs) const;
Point &operator+=(const Point<P> &rhs);
Point operator-(const Point<P> &rhs) const;
Point &operator-=(const Point<P> &rhs);
```

##### distanceTo

```cpp
Precision distanceTo(const Point<P> &rhs) const;
```
Returns the distance to another point.

## Rect

### Members

##### Coordinates

```cpp
Point<Precision> position;
Point<Precision> size;
```

##### Operators

```cpp
bool operator==(const Rect<P> &rhs) const;
bool operator!=(const Rect<P> &rhs) const;
```

##### intersect

```cpp
bool intersect(const Rect &other) const
```
Returns whether this rectangle intersects with `other`.
