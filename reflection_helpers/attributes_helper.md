# [attributes_helper](attributes_helper.hpp)

Helper functions to manipulate [runtime attributes](Attributes.md).

### getRuntimeAttributes

```cpp
template<typename T>
const Attributes & getRuntimeAttributes() noexcept;
```

Returns the runtime attributes for `T`.

### 