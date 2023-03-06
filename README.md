# putils

[![tests](https://github.com/phisko/putils/workflows/tests/badge.svg)](https://github.com/phisko/putils/actions/workflows/tests.yml)

Phisko's C++ Utilities

## [Reflection](https://github.com/phisko/reflection)

A simple, stand-alone, header-only and easily pluggable reflection system.

This does not allow you to introspect any type provided by any library. It simply provides an API that you can implement in your own classes so that you can introspect them.
 
Good examples can be found in the [imgui_helper](putils/reflection_helpers/imgui_helper.md) and [json_helper](putils/reflection_helpers/json_helper.md) utilities.

Another example is the [register_type](putils/lua/README.md) function provided for lua, that inspects a type and registers all its attributes and methods to a lua state.

## Utility classes

* [color & normalized_color](putils/color.md): helper classes to manipulate colors
* [ini_file](putils/ini_file.md): `.ini` file parser
* [observable](putils/observable.md): A minimalistic (but greatly sufficient) implementation of the observer pattern.
* [point](putils/point.md): Simple abstraction to a 2D or 3D point.
* [rect](putils/rect.md): Simple abstraction to a 2D or 3D rectangle.
* [raii](putils/raii.md): A class template able to perform RAII on any type.
* [spin_lock](putils/spin_lock.md): Spin lock implementation, "sleep-free" alternative to mutexes.
* [timer](putils/timer.md): Simple timer based on std::chrono.

## Standard library replacements

* [string](putils/string.hpp): fixed-size drop-in replacement for `std::string`
* [vector](putils/vector.hpp): fixed-size drop-in replacement for `std::vector`
* [function](putils/function.hpp): fixed-size drop-in replacement for `std::function`

## Packages

* [Lua](putils/lua/README.md) (enabled with the `PUTILS_LUA` CMake option)
* [Python](putils/python/README.md) (enabled with the `PUTILS_PYTHON` CMake option)
* [Meta](https://github.com/phisko/meta)
* [plugin_manager](putils/plugin_manager/README.md) (enabled with the `PUTILS_PLUGIN_MANAGER` CMake option)
* [Reflection](https://github.com/phisko/reflection)
* [fmt](putils/fmt/): formatters and concepts for [fmt](https://github.com/fmtlib/fmt)
* [scn](putils/scn/): scanners and concepts for [scn](https://github.com/eliaskosunen/scnlib)

## Function-only headers

* [angle](putils/angle.md): helper functions when manipulating angles
* [anonymous_variable](putils/anonymous_variable.md): macro to define an anonymous variable
* [casts](putils/casts.md): static and dynamic casts for std::unique_ptr
* [chop](putils/chop.md): function removing any initial or trailing whitespace, as well as surrounding quotes, from a string
* [clamp](putils/clamp.md): function that clamps a value between two bounds
* [command_line_arguments](putils/command_line_arguments.md): function that parses command-line arguments into a struct
* [concatenate](putils/concatenate.md): function taking any number of parameters and concatenating them into a string
* [curl](putils/curl.md): function that runs a `curl` request over CLI (to avoid having to install curl packages. This is NOT the optimal way to do an HTTP request)
* [default_constructors](putils/default_constructors.md): macros to avoid redundant declarations of default constructors
* [epsilon](putils/epsilon.md): approximate comparison of floating point types
* [forward_to](putils/forward_to.md): helper macros to forward arguments to an object
* [get_call_stack](putils/get_call_stack.md): function which returns a formatted call-stack
* [get_last_error_as_string](putils/get_last_error_as_string.md): function for Windows
* [hash](putils/hash.md): pre-defined hashers for enums and pairs
* [lengthof](putils/lengthof.md): template function returning the length of an array
* [macro_as_string](putils/macro_as_string.md): returns the value of a macro as a string
* [magica_voxel](putils/magica_voxel.md): structure declarations for the [magica_voxel format](https://ephtracy.github.io/)
* [magica_voxel_to_polyvox_loader](putils/magica_voxel_to_polyvox_loader.md): conversion functions to parse the magica_voxel format into [PolyVox](https://bitbucket.org/volumesoffun/polyvox/src/develop/) objects
* [on_scope_exit](putils/on_scope_exit.md): returns an object that calls a function in its dtor
* [parse](putils/parse.md): uses a `std::stringstream` to parse an object
* [profiling.hpp](putils/profiling.hpp): macros wrapping [Tracy](https://github.com/wolfpld/tracy)
* [rand](putils/rand.md): function using C++ 11's random number generation facilities
* [range](putils/range.md): macro return the `begin` and `end` iterators of a container
* [read_stream](putils/read_stream.md): function which extracts an entire std::istream to a std::string
* [regex](putils/regex.md): utilities to easily construct and match regexes through literal operators
* [same_sign](putils/same_sign.md): function indicating whether two scalars have the same sign
* [thread_name](putils/thread_name.md): sets or gets the OS-level name for the current thread
* [scoped_setter](putils/scoped_setter.md): sets the value of a variable for the duration of the current scope
* [sign](putils/sign.md): function returning -1, 0 or 1 depending on the sign of the parameter
* [split](putils/split.md): split a string according to a delimiter
* [static_assert](putils/static_assert.md): type template to let `static_assert` fail for a specific type
* [url](putils/url.md): URL encoder and decoder
* [visit](putils/visit.md): `overloaded` type letting you pass multiple lambdas to `std::visit`
* [with](putils/with.md): `with` macro mimicking the Kotlin construct