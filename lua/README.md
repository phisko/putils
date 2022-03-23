# lua

CMake "interface library" linking against `lua` and `sol2`, as well as some helpers.

* [registerType](lua_helper.hpp): a template function that, given a `Reflectible` type `T`, registers it by its name, providing all its member functions and attributes, to a sol::state