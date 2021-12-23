# [runtime_helper](runtime_helper.hpp)

Helper functions to manipulate [runtime attributes](Attributes.md).

### getRuntimeAttributes

```cpp
template<typename T>
const Attributes & getAttributes() noexcept;
```

Returns the runtime attributes for `T`.

### findAttribute

```cpp
const AttributeInfo * findAttribute(const Attributes & attributes, std::string_view path, std::string_view separator) noexcept;

template<typename T>
const AttributeInfo * findAttribute(std::string_view path, std::string_view separator) noexcept;
```

Attempts to find the attribute specified by `path`, where `separator` is used to index subobjects.