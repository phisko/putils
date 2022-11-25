# [regex](regex.hpp)

Helper functions and string extensions for regex matching.

## ""_r

```cpp
std::regex operator ""_r(const char * str, size_t) noexcept;
```

Constructs an `std::regex` from a string literal.

## ""_m

```cpp
std::pair<std::regex, bool> operator ""_m(const char * str, size_t) noexcept;
```

Constructs a "matching group" from a string literal. This matching group can then be used with the following comparison operator to extract a match.

## Comparison operators

```cpp
bool operator==(const char *& str, const std::regex & reg) noexcept;
std::cmatch operator==(const char * str, const std::pair<std::regex, bool> & reg) noexcept;
```

Comparing a string with an `std::regex` returns a boolean indicating whether the string matches the regex.
Comparing a string with a "matching group" (generated with `""_m`) returns the match results with the regex.