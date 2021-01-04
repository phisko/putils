# [Color & NormalizedColor](Color.hpp)

[Reflectible](reflection.md) helper classes to manipulate colors.

## Color

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

## NormalizedColor

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

### toColor, toNormalizedColor

```cpp
Color toColor(const NormalizedColor & normalized) noexcept;
NormalizedColor toNormalizedColor(const Color & color) noexcept;
```

### toRGBA, fromRGBA

```cpp
unsigned int toRGBA(const NormalizedColor & color) noexcept;
NormalizedColor fromRGBA(unsigned int color) noexcept;
```

### toARGB, fromARGB

```cpp
unsigned int toARGB(const Color & color) noexcept;
unsigned int toARGB(const NormalizedColor & color) noexcept;
Color fromARGB(unsigned int color) noexcept;
```

Multiplication and division operators are also defined between color types and `float`s.