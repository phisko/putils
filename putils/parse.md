# [parse](parse.hpp)

```cpp
template<typename Obj>
void parse(Obj & obj, std::string_view str) noexcept;;
```

Parses `str` into `obj` using `std::stringstream`.