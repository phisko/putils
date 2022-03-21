# [Command-line arguments](command_line_arguments.hpp)

## parseArguments

```cpp
template<typename T>
T parseArguments(std::span<const std::string_view> args, std::string_view help = "") noexcept;
```

Given a [reflectible](reflection/README.md) type `T`, parses the command-line arguments into its attributes. Short flags can be specified for each option through the `"flag"` metadata.

If the command-line contains `--help` or `-h`, print a help message. Option-specific help messages can be specified through the `"help"` metadata. If `help` is not empty, it will be printed before the help message.

## toArgumentVector

```cpp
std::vector<std::string_view> toArgumentVector(int ac, const char * const * av) noexcept;
```

Converts the C-style `ac, av` parameters to a vector of `string_views` which can be passed to `parseArguments`.