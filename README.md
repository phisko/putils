# putils

[![tests](https://github.com/phisko/putils/workflows/tests/badge.svg)](https://github.com/phisko/putils/actions/workflows/tests.yml)

Phisko's C++ Utilities

## [Reflection](https://github.com/phisko/reflection)

A simple, stand-alone, header-only and easily pluggable reflection system.

This does not allow you to introspect any type provided by any library. It simply provides an API that you can implement in your own classes so that you can introspect them.
 
Good examples can be found in the [imgui_helper](reflection/imgui_helper.md) and [json_helper](reflection/json_helper.md) utilities.

Another example is the [registerType](lua/README.md) function provided for lua, that inspects a type and registers all its attributes and methods to a lua state.

## Utility classes

* [Color & NormalizedColor](Color.md): helper classes to manipulate colors
* [Directory](Directory.md): An abstraction to Unix and Windows directories, which lets you iterate through a directory and get information about each file.
* [IniFile](IniFile.md): `.ini` file parser
* [Observable](Observable.md): A minimalistic (but greatly sufficient) implementation of the Observer pattern.
* [Point](Point.md): Simple abstraction to a 2D or 3D point.
* [Rect](Rect.md): Simple abstraction to a 2D or 3D rectangle.
* [RAII](RAII.md): A class template able to perform RAII on any type.
* [SpinLock](SpinLock.md): Spin lock implementation, "sleep-free" alternative to mutexes.
* [ThreadPool](ThreadPool.hpp): Thread pool implementation
* [Timer](Timer.md): Simple timer based on std::chrono.

## Standard library replacements

* [string](string.hpp): fixed-size drop-in replacement for `std::string`
* [vector](vector.hpp): fixed-size drop-in replacement for `std::vector`
* [function](function.hpp): fixed-size drop-in replacement for `std::function`

## Packages

* [Lua](lua/README.md) (enabled with the PUTILS_LUA CMake option)
* [Python](python/README.md) (enabled with the PUTILS_PYTHON CMake option)
* [Mediator](mediator/README.md) (enabled with the PUTILS_MEDIATOR CMake option)
* [Meta](meta/README.md)
* [PluginManager](pluginManager/README.md) (enabled with the PUTILS_PLUGIN_MANAGER CMake option)
* [Reflection](reflection/README.md)
* [Vulkan](vulkan) (enabled with the PUTILS_VULKAN CMake option)

## Function-only headers

* [angle](angle.md): helper functions when manipulating angles
* [anonymous_variable](anonymous_variable.md): macro to define an anonymous variable
* [casts](casts.md): static and dynamic casts for std::unique_ptr
* [chop](chop.md): function removing any initial or trailing whitespace, as well as surrounding quotes, from a string
* [clamp](clamp.md): function that clamps a value between two bounds
* [command_line_arguments](command_line_arguments.md): function that parses command-line arguments into a struct
* [concatenate](concatenate.md): function taking any number of parameters and concatenating them into a string
* [curl](curl.md): function that runs a `curl` request over CLI (to avoid having to install curl packages. This is NOT the optimal way to do an HTTP request)
* [epsilon](epsilon.md): approximate comparison of floating point types
* [erase](erase.md): function taking a container and a value and erasing the value from the container
* [file_extension](file_extension.md): functions for working with file extensions
* [get_call_stack](get_call_stack.md): function which returns a formatted call-stack
* [get_current_dir](get_current_dir.md): function which returns the current working directory as a string
* [get_last_error_as_string](get_last_error_as_string.md): function for Windows
* [go_to_bin_dir](go_to_bin_dir.md): function which takes `argv[0]` as parameter and changes the current working directory to that containing the executed file.
* [hash](hash.md): pre-defined hashers for enums and pairs
* [lengthof](lengthof.md): template function returning the length of an array
* [macro_as_string](macro_as_string.md): returns the value of a macro as a string
* [MagicaVoxel](MagicaVoxel.md): structure declarations for the [MagicaVoxel format](https://ephtracy.github.io/)
* [MagicaVoxelToPolyvoxLoader](MagicaVoxelToPolyvoxLoader.md): conversion functions to parse the MagicaVoxel format into [PolyVox](https://bitbucket.org/volumesoffun/polyvox/src/develop/) objects
* [members](members.md): help with class members
* [on_scope_exit](on_scope_exit.md): returns an object that calls a function in its dtor
* [rand](rand.md): function using C++ 11's random number generation facilities
* [read_stream](read_stream.md): function which extracts an entire std::istream to a std::string
* [regex](regex.md): utilities to easily construct and match regexes through literal operators
* [remove_if](remove_if.md): function taking a container and a predicate and calling std::remove_if
* [same_sign](same_sign.md): function indicating whether two scalars have the same sign
* [thread_name](thread_name.md): sets or gets the OS-level name for the current thread
* [scoped_setter](scoped_setter.md): sets the value of a variable for the duration of the current scope
* [sign](sign.md): function returning -1, 0 or 1 depending on the sign of the parameter
* [static_assert](static_assert.md): type template to let `static_assert` fail for a specific type
* [to_string](to_string.md): uses a `std::stringstream` to serialize an object
* [traits](traits.md): type traits
* [url](url.md): URL encoder and decoder
* [universal_functor](universal_functor.md): macro for passing generic functions as functors
* [visit](visit.md): `overloaded` type letting you pass multiple lambdas to `std::visit`
* [with](with.md): `with` macro mimicking the Kotlin construct

## CMake tools

* [conan](CMakeModules/conan.cmake): lets you run conan commands directly from cmake
* [conan_helper](CMakeModules/conan_helper.cmake): provides a helper macro to simply list conan dependencies
* [cotire](https://github.com/sakra/cotire): lets you easily set up precompiled-header and single-compilation-unit compilations to speed up builds
