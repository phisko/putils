# [IniFile](IniFile.hpp)

Representation of a parsed `.ini` file (sections containing key-value pairs).

## Members

### Section

```cpp
struct Section {
    std::unordered_map<std::string, std::string> values;
};
```

Structure representing a section.

### sections

```cpp
std::unordered_map<std::string, Section> sections;
```

Maps a section's name to its content.

## Helper function

### parseIniFile

```cpp
IniFile parseIniFile(const char * file) noexcept;
```

Opens `file` and parses it.