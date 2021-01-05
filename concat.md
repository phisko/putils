# [concat](concat.hpp)

```cpp
template<typename ... Args>
std::string concat(Args && ... args) noexcept;
```

Concatenates `args` into a single string using `std::ostringstream`.