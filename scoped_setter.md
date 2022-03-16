# [scoped_setter](scoped_setter.hpp)

```cpp
template<typename T, typename U>
auto setForScope(T & dest, U && src) noexcept {
```

Sets `dest` to `src` and returns an object that will reset `dest` to its original value in its destructor.
