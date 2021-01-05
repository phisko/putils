# [universal_functor](universal_functor.hpp)

```cpp
#define UNIVERSAL_FUNCTOR(func) [](auto && ... args) { func(FWD(args)...); }
```

Allows passing a templated function as a functor.