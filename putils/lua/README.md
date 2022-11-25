# lua

CMake "interface library" linking against `lua` and `sol2`, as well as some helpers.

* [register_type](lua_helper.hpp): a template function that, given a [reflectible](https://github.com/phisko/reflection) type `T`, registers it by its name, providing all its member functions and attributes, to a `sol::state`.