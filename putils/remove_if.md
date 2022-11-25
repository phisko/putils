# [remove_if](remove_if.hpp)

```cpp
template<typename Container, typename Predicate>
void remove_if(Container & container, Predicate && pred) noexcept;
```

Removes all items in `container` for which `pred(obj)` returns `true`.