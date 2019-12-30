# [Library](UnixLibrary.hpp)

The "Library" class family are abstractions to Unix and Windows DLLs. These should not be explitly instantiated, but instead created by `LibraryFactory::make`

## Members

### execute
```cpp
T execute(const std::string &name, P &&...args)
```
Execute the `name` function, returning T and taking `args` as parameters.

## Virtual members

These are the functions to be redefined if a new type of library is to be supported.

### loadSymbol

```cpp
virtual void *loadSymbol(const std::string &name) = 0;
```

Equivalent of `dlsym`, loads a function pointer with the given name from the library.
