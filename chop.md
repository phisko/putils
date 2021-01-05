# [chop](chop.hpp)

```cpp
template<size_t N = 0>
string chop(std::string_view str) noexcept;
```

Returns `str` after having chopped off any trailing whitespace. Returns either an `std::string`, or ` putils::string<N>` if `N` was specified.