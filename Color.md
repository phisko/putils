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
Color toColor(const NormalizedColor & normalized);
NormalizedColor toNormalizedColor(const Color & color);
```

### toRGBA, fromRGBA

```cpp
unsigned int toRGBA(const NormalizedColor & color);
NormalizedColor fromRGBA(unsigned int color);
```

### toARGB, fromARGB

```cpp
unsigned int toARGB(const Color & color);
unsigned int toARGB(const NormalizedColor & color);
Color fromARGB(unsigned int color);
```

Multiplication and division operators are also defined between color types and `float`s.