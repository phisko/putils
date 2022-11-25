# [Command-line arguments](command_line_arguments.hpp)

## parse_arguments

```cpp
template<typename T>
T parse_arguments(std::span<const std::string_view> args, std::string_view help = "") noexcept;
```

Given a [reflectible](https://github.com/phisko/reflection) type `T`, parses the command-line arguments into its attributes. Short flags can be specified for each option through the `"flag"` metadata.

If the command-line contains `--help` or `-h`, print a help message. Option-specific help messages can be specified through the `"help"` metadata. If `help` is not empty, it will be printed before the help message.

## to_argument_vector

```cpp
std::vector<std::string_view> to_argument_vector(int ac, const char * const * av) noexcept;
```

Converts the C-style `ac, av` parameters to a vector of `string_views` which can be passed to `parse_arguments`.