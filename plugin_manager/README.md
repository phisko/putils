# plugin_manager
A simple and re-usable C++ dynamic library abstraction.

## EXPORT
Export.hpp contains an `EXPORT` macro which should be added to the prototype of DLL functions to be exported.
```cpp
EXPORT void someFunction() {}
```

## Classes

* [Library](Library.md): abstraction to a dynamically loaded library.
* [PluginManager](PluginManager.md): object which manages and simplifies usage of several `Libraries`.

