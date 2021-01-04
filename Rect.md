# [Rect](Rect.hpp)

Abstraction to a 2D/3D rectangle.

## Members

### Coordinates

```cpp
Point<Precision> position;
Point<Precision> size;
```

### Operators

```cpp
bool operator==(const Rect<P> &rhs) const;
bool operator!=(const Rect<P> &rhs) const;
```