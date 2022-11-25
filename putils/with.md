# [with](with.hpp)

```cpp
#define putils_with(object) auto & _(object);
```

Lets users elegantly indicate that an object should be seen as "`this`" in the current scope, with an alias to it called `_`.