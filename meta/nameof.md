# [nameof](nameof.hpp)

```cpp
#define putils_nameof(name) #name
#define putils_nameof_private(name) std::string_view(#name).substr(1)
```

Macros for explicitly stating that we are extracting a class or attribute's name. `private` variation skips the first character (typically `_` or `m`).