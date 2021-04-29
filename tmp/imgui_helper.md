# [imgui_helper](imgui_helper.hpp)

Helper functions for manipulating reflectible types in `ImGui`.

## imguiEdit

```cpp
template<typename T>
void imguiEdit(T & obj);
```

For each reflectible attribute in `obj`, displays it in an editable box (or a colorpicker for colors).

## imguiDisplay

```cpp
template<typename T>
void imguiDisplay(const T & obj);
```

For each reflectible attribute in `obj`, displays it.

## imguiEnumCombo

```cpp
template<typename E>
void imguiEnumCombo(E & e);
```

Displays a combo box letting users pick among all the enum values of `E`.