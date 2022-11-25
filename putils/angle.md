# [angle](angle.hpp)

Helper functions for angle manipulation.

## pi

```cpp
static constexpr auto pi = 3.14159265359f;
```

## constrain_angle

```cpp
float constrain_angle(float angle) noexcept;
```

Returns `angle` constrained to the `[-pi, pi]` range.

## direction component extractors

```cpp
float get_yaw_from_normalized_direction(const vec3f & dir) noexcept;
float get_pitch_from_normalized_direction(const vec3f & dir) noexcept;
float get_2d_angle_from_normalized_direction(const vec2f & dir) noexcept;
```

Extracts the desired component from `dir`, where `dir` is a normalized direction vector.

## Unit converters

```cpp
float to_degrees(float rads) noexcept;
float to_radians(float degs) noexcept;
```