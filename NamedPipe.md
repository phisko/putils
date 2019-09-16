# [NamedPipe](UnixNamedPipe.hpp)

Abstraction to named pipes.

### Members

##### Constructor
```cpp
UnixNamedPipe(const std::string &fileName);
```

##### open
```cpp
void open(const std::string &fileName, std::ifstream::openmode mode = std::ifstream::in | std::ifstream::out);
```
Opens a new named pipe in the given mode.

##### close
```cpp
void close(bool erase = true)
```
Closes the named pipe and optionally erases it.
