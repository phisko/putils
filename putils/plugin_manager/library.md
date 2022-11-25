# [library](unix_library.hpp)

The "library" class family are abstractions to Unix and Windows DLLs. These should not be explitly instantiated, but instead created by [library_factory::make](library_factory.hpp).

## Members

### execute
```cpp
T execute(const std::string & name, P && ... args)
```
execute the `name` function, returning T and taking `args` as parameters.

## Virtual members

These are the functions to be redefined if a new type of library is to be supported.

### load_symbol

```cpp
virtual void * load_symbol(const std::string & name) = 0;
```

Equivalent of `dlsym`, loads a function pointer with the given name from the library.
