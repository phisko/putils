# [has_type](has_type.hpp)

```cpp
template<typename T, typename ...Types>
struct has_type;
```

Inherits from `std::true_type` if `Types...` contains `T`.