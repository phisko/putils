# [visit](visit.hpp)

```cpp
template<typename ... Ts>
struct overloaded;

template<typename ... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
```

Allows to pass a series of lambdas to `std::visit` as a single object that will use all their function call operators.