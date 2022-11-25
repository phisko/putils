# [concatenate](concatenate.hpp)

```cpp
template<putils::streamable<std::stringstream> ... Args>
std::string concatenate(Args && ... args) noexcept;
```

Concatenates `args` into a single string using `std::stringstream`.