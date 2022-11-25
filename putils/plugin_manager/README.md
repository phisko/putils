# plugin_manager
A simple and re-usable C++ dynamic library abstraction.

## EXPORT

[export.hpp](export.hpp) contains an `EXPORT` macro which should be added to the prototype of DLL functions to be exported.

```cpp
EXPORT void some_function() {}
```

## Classes

* [library](library.md): abstraction to a dynamically loaded library.
* [plugin_manager](plugin_manager.md): object which manages and simplifies usage of several `libraries`.

