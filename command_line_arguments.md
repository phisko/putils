# [Command-line arguments](command_line_arguments.hpp)

```cpp
template<typename T>
T parseArguments(int ac, const char ** av) noexcept;
```

Given a [reflectible](reflection/README.md) type `T`, parses the command-line arguments into its attributes.