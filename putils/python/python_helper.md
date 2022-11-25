# [python_helper](python_helper.hpp)

```cpp
using py = pybind11;

template<typename T>
void register_type(py::module_ & m) noexcept;
```

Helper function that registers a [reflectible](https://github.com/phisko/reflection) type with a `pybind11` module.