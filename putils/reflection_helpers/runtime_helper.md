# [runtime_helper](runtime_helper.hpp)

Helper functions to manipulate [runtime attributes](attributes.md).

### get_attributes

```cpp
template<typename T>
const attributes & get_attributes() noexcept;
```

Returns the runtime attributes for `T`.

### find_attribute

```cpp
const attribute_info * find_attribute(const attributes & attributes, std::string_view path, std::string_view separator) noexcept;

template<typename T>
const attribute_info * find_attribute(std::string_view path, std::string_view separator) noexcept;
```

Attempts to find the attribute specified by `path`, where `separator` is used to index subobjects.