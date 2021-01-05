# [apply](apply.hpp)

```cpp
template<typename F, typename ...Args>
std::result_of_t<F(Args...)> apply(const std::tuple<Args...> & tuple, F && f) noexcept;
```

Calls `f` with the elements of `tuple` as arguments.