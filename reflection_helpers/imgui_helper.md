# [imgui_helper](imgui_helper.hpp)

Helper functions for manipulating reflectible types in `ImGui`.

## imguiEdit

```cpp
template<typename T>
void imguiEdit(T && obj);

template<typename T>
void imguiEdit(const char * label, T && obj);
```

Displays `obj`, in an editable way if `obj` is not const.

Currently supported types:
* all [reflectible](../reflection/README.md) types
* strings (both `std::string` and [putils::string](../string.md))
* iterable containers
* [putils::Color & putils::NormalizedColor](../Color.md)
* functors, with editable arguments
* enums
* bool
* int
* unsigned int
* float
* double

## imguiEnumCombo

```cpp
template<typename E>
void imguiEnumCombo(E & e);
```

Displays a combo box letting users pick among all the enum values of `E`.