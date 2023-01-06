# [static_assert](static_assert.hpp)

```cpp
#define putils_static_assert_false(T)
```

Allows you to do a `static_assert(false)` that only fails for `T`.

## Helper type

```cpp
template<typename T>
struct always_false : std::false_type {};
```

Lets you have an equivalent of `static_assert(false)` in template code. `static_assert(false)` will _always_ cause a compilation error, so having this depend on a type parameter gives you a way to explicitly fail for a given type.