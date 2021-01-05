# [new_from_tuple](new_from_tuple.hpp)

```cpp
template<class T, class Tuple>
std::unique_ptr<T> new_from_tuple(Tuple && t) noexcept;
```

Creates a `T`, passing the elements of `t` as ctor arguments.