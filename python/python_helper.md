# [python_helper](python_helper.hpp)

```cpp
using py = pybind11;

template<typename T>
void registerType(py::module_ & m) noexcept;
```

Helper function that registers a [reflectible](../reflection.md) type with a `pybind11` module.