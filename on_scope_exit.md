# [on_scope_exit](on_scope_exit.hpp)

```cpp
template<typename Func>
auto onScopeExit(Func && f) noexcept;
```

Returns an object that will automatically call `f` in its destructor.