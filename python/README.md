# Python

pybind11, a header-only encapsulation of Python <-> C++ bindings, along with some helpers.

* registerType: a template function that, given a `Reflectible` type `T`, registers it by its name, providing all its member functions and attributes, to a sol::state

Linking with the `puthon` CMake target will add all necessary include directories to your include path.
