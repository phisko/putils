# [chop](chop.hpp)

```cpp
template<size_t N = 0>
string chop(std::string_view str) noexcept;
```

Returns `str` after having chopped off any initial or trailing whitespace, as well as surrounding quotes. Returns either an `std::string_view`, or ` putils::string<N>` if `N` was specified.