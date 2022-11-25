# [bit_field_ref](bit_field_ref.hpp)

Allows manipulation of a pointer's data through its bits.

## Members

### Ctor

```cpp
bit_field_ref(void * data) noexcept;
```

### get

```cpp
bool get(size_t index) const noexcept;
bool get(size_t byte_index, size_t bit_index) const noexcept;
```

Returns whether the specified bit is set.

### set

```cpp
void set(size_t index, bool value) noexcept;
void set(size_t byte_index, size_t bit_index, bool value) noexcept;
```

Sets the specified bit to `value`.

### toggle

```cpp
void toggle(size_t index) noexcept;
void toggle(size_t byte_index, size_t bit_index) noexcept;
```

Toggles the specified bit's value.