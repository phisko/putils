# [epsilon](epsilon.hpp)

```cpp
template<typename T>
bool epsilon_equals(T a, T b) noexcept;
```

Returns whether `a` and `b` are "approximately equal", meaning the difference between them is smaller than the epsilon for `T`.