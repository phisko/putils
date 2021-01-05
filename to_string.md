# [to_string](to_string.hpp)

## toString

```cpp
template<typename Obj>
std::string toString(Obj && obj) noexcept;
```

Serializes `obj` using `std::stringstream`.

## parse

```cpp
template<typename Obj>
void parse(Obj & obj, const char * str) noexcept;;
```

Parses `str` into `obj` using `std::stringstream`.