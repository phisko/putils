# [plugin_manager](plugin_manager.hpp)

Manages a set of DLLs.

## Members

### Constructor
```cpp
plugin_manager(const std::string & path) noexcept
```
Loads all libraries (.so/.dll) files in the directory specified by `path`.

### execute
```cpp
void execute(const std::string & name, P && ... params);
```
Execute, in all DLLs, the `name` function, passing `params` as parameters

### execute_with_return
```cpp
std::vector<T> execute_with_return(const std::string &name, P && ... params);
```
Execute, in all DLLs, the `name` function returning `T`, passing `params` as parameters.
Return values of all functions are returned in an `std::vector`.
