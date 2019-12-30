# [PluginManager](PluginManager.hpp)

Manages a set of DLLs.

## Members

### Constructor
```cpp
PluginManager(const std::string &path) noexcept
```
Loads all libraries (.so/.dll) files in the directory specified by `path`.

### execute
```cpp
void execute(const std::string &name, P &&...params);
```
Execute, in all DLLs, the `name` function taking `Args` as parameters

### executeWithReturn
```cpp
std::vector<T> executeWithReturn(const std::string &name, P &&...params);
```
Execute, in all DLLs, the `name` function returning `T` and taking `Args` as parameters.
Return values of all functions are returned in an `std::vector`.
