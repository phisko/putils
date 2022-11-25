# [color & normalized_color](color.hpp)

[Reflectible](https://github.com/phisko/reflection) helper classes to manipulate colors.

## color

Color formatted as a union of 4 `unsigned char`s and an `unsigned int`.

### Members

```cpp
union {
    unsigned char attributes[4];
    struct {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };
    unsigned int rgba;
};
```

## normalized_color

Color formatted as 4 `float`s, normalized between 0 and 1.

### Members

```cpp
union {
    float attributes[4];
    struct {
        float r;
        float g;
        float b;
        float a;
    };
};
```

## Helper functions

### to_color, to_normalized_color

```cpp
color to_color(const normalized_color & normalized) noexcept;
normalized_color to_normalized_color(const color & color) noexcept;
```

### to_rgba, from_rgba

```cpp
unsigned int to_rgba(const normalized_color & color) noexcept;
normalized_color from_rgba(unsigned int color) noexcept;
```

### to_argb, from_argb

```cpp
unsigned int to_argb(const color & color) noexcept;
unsigned int to_argb(const normalized_color & color) noexcept;
color from_argb(unsigned int color) noexcept;
```

Multiplication and division operators are also defined between color types and `float`s.