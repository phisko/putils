# [casts](casts.hpp)

```cpp
template<typename Derived, typename Base>
std::unique_ptr<Derived> static_unique_ptr_cast(std::unique_ptr<Base> && p) noexcept;

template<typename Derived, typename Base>
std::unique_ptr<Derived> dynamic_unique_ptr_cast(std::unique_ptr<Base> && p) noexcept;
```

Functions to cast `unique_ptrs` from one type to another.