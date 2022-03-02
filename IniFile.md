# [IniFile](IniFile.hpp)

Representation of a parsed `.ini` file (sections containing key-value pairs).

## Members

### Section

```cpp
struct Section {
    std::map<std::string, std::string> values;
    std::map<std::string, Section> sections;
};
```

Structure representing a section.

### sections

```cpp
std::map<std::string, Section> sections;
```

Maps a section's name to its content.