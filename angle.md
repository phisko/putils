# [angle](angle.hpp)

Helper functions for angle manipulation.

## pi

```cpp
static constexpr auto pi = 3.14159265359f;
```

## constrainAngle

```cpp
float constrainAngle(float angle) noexcept;
```

Returns `angle` constrained to the `[-pi, pi]` range.

## Direction component extractors

```cpp
float getYawFromNormalizedDirection(const Vector3f & dir) noexcept;
float getPitchFromNormalizedDirection(const Vector3f & dir) noexcept;
float get2DAngleFromNormalizedDirection(const Vector2f & dir) noexcept;
```

Extracts the desired component from `dir`, where `dir` is a normalized direction vector.

## Unit converters

```cpp
float toDegrees(float rads) noexcept;
float toRadians(float degs) noexcept;
```