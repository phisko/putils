# lua

sol, a single header lua encapsulation, along with a few helpers.

* [registerType](lua_helper.hpp): a template function that, given a `Reflectible` type `T`, registers it by its name, providing all its member functions and attributes, to a sol::state

Linking with the `plua` CMake target will link your project with the Lua libraries and add all necessary include directories to your include path.
