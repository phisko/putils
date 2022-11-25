# [rect](rect.hpp)

Abstraction to a 2D/3D rectangle.

## Members

### Coordinates

```cpp
point<Precision> position;
point<Precision> size;
```

### Operators

```cpp
bool operator==(const rect<P> &rhs) const;
bool operator!=(const rect<P> &rhs) const;
```