# [imgui_helper](imgui_helper.hpp)

Helper functions for manipulating [reflectible](https://github.com/phisko/reflection) types in `ImGui`.

## imgui_edit

```cpp
template<typename T>
bool imgui_edit(T && obj);

template<typename T>
bool imgui_edit(const char * label, T && obj);
```

Displays `obj`, in an editable way if `obj` is not const. Returns whether `obj` was modified.

Currently supported types:
* all `reflectible` types
* strings (both `std::string` and [putils::string](../string.md))
* iterable containers
* [putils::color & putils::normalized_color](../color.md)
* functors, with editable arguments
* enums
* bool
* int
* unsigned int
* float
* double

## imgui_enum_combo

```cpp
template<typename E>
void imgui_enum_combo(E & e);
```

Displays a combo box letting users pick among all the enum values of `E`.