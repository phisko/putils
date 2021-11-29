# [BitFieldRef](BitFieldRef.hpp)

Allows manipulation of a pointer's data through its bits.

## Members

### Ctor

```cpp
BitFieldRef(void * data) noexcept;
```

### get

```cpp
bool get(size_t index) const noexcept;
bool get(size_t byteIndex, size_t bitIndex) const noexcept;
```

Returns whether the specified bit is set.

### set

```cpp
void set(size_t index, bool value) noexcept;
void set(size_t byteIndex, size_t bitIndex, bool value) noexcept;
```

Sets the specified bit to `value`.

### toggle

```cpp
void toggle(size_t index) noexcept;
void toggle(size_t byteIndex, size_t bitIndex) noexcept;
```

Toggles the specified bit's value.