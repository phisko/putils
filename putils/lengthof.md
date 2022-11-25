# [lengthof](lengthof.hpp)

```cpp
template<typename T>
constexpr size_t lengthof() noexcept;

template<typename T, size_t N>
constexpr size_t lengthof(const T(&)[N]) noexcept;
```

Functions for extracting the length of an array.