# [rand](rand.hpp)

```cpp
template<typename T>
T rand(T start = 0, T end = std::numeric_limits<T>::max) noexcept;
```

Returns a random value between `start` and `end`, using `std::random_device`, `std::default_random_engine` and `std::uniform_*_distribution`.