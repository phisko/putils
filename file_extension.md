# [file_extension](file_extension.hpp)

Helper functions for manipulating paths.

```cpp
std::string_view get_directory(const char * file) noexcept;
std::string_view remove_directory(const char * file) noexcept;
std::string_view file_extension(const char * file) noexcept;
std::string_view remove_extension(const char * file) noexcept;
```