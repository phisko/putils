# [Directory](ADirectory.hpp)

An abstraction to Unix and Windows directories, which lets you iterate through a directory and get information about each file.

## Members

### File
```cpp
struct File
{
    std::string name;
    std::string fullPath;
    bool isDirectory;
};
```
Abstraction to a file, specifying name, full path and whether this is a directory.

### getNextFile
```cpp
std::unique_ptr<File> getNextFile() noexcept;
```
Returns a pointer to a `File`, or `nullptr` if the end of the directory has been reached.

### getFiles
```cpp
std::vector<File> getFiles() noexcept;
```
Returns a vector of all the files in the directory.

### for_each
```cpp
template<typename Func> // Func: void(const File &)
void for_each(Func && func) noexcept;
```
Applies a function to all files in the directory.
