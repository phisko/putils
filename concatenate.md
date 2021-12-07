# [concatenate](concatenate.hpp)

```cpp
template<typename ... Args>
std::string concatenate(Args && ... args) noexcept;
```

Concatenates `args` into a single string using `std::ostringstream`.