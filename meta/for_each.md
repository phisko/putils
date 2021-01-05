# [for_each](for_each.hpp)

Helper functions to iterate over things.

## for_each_type

```cpp
template<typename ...Types, typename Func>
constexpr void for_each_type(Func && f) noexcept;
```

For each type `T` in `Types...`, calls `f(putils::meta::type<T>)`. Functions can then extract `T` using [putils_wrapped_type](type.md).

## tuple_for_each

```cpp
template<typename F, typename ...Args>
constexpr void tuple_for_each(std::tuple<Args...> & tuple, F && f) noexcept;

template<typename F, typename ...Args>
constexpr void tuple_for_each(const std::tuple<Args...> & tuple, F && f) noexcept;
```

For each element `e` in `tuple`, calls `f(e)`.