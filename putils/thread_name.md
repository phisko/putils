# [thread_name](thread_name.hpp)

```cpp
void set_thread_name(const wchar_t * name) noexcept;
```

Sets the OS-level name for the current thread.

```cpp
const std::string & get_thread_name() noexcept;
```

Returns the OS-level name for the current thread.
