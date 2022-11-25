# [ini_file](ini_file.hpp)

Representation of a parsed `.ini` file (sections containing key-value pairs).

## Members

### section

```cpp
struct section {
    std::map<std::string, std::string> values;
    std::map<std::string, section> sections;
};
```

Structure representing a section.

### sections

```cpp
std::map<std::string, section> sections;
```

Maps a section's name to its content.