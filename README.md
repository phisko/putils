# putils

Phisko's C++ Utilities

## [Reflection](reflection.md)

A simple, stand-alone, header-only and easily pluggable reflection system.

This does not allow you to introspect any type provided by any library. It simply provides an API that you can implement in your own classes so that you can introspect them.
 
A good example of this is the [Serializable](reflection/Serializable.md) class, that uses reflection to inspect itself and serialize its attributes.

Another example is the [registerType](lua/README.md) function provided for lua, that inspects a type and registers all its attributes and methods to a lua state.

## Utility classes

* [AbstractFactory](AbstractFactory.md): Base class for a factory able to create any type in a given typelist calling ``make<T>()``.
* [Color & NormalizedColor](Color.md): helper classes to manipulate colors
* [Directory](Directory.md): An abstraction to Unix and Windows directories, which lets you iterate through a directory and get information about each file.
* [json](json.hpp): JSON implementation, credit goes to [nlohmann](https://github.com/nlohmann/json)
* [magic_enum](magic_enum.hpp): functions to convert enums to strings, get enum counts... credit goes to [Neargye](https://github.com/Neargye/magic_enum)
* [Observable](Observable.md): A minimalistic (but greatly sufficient) implementation of the Observer pattern.
* [Point](Point.md): Simple abstraction to a 2D or 3D point/rectangle.
* [RAII](RAII.md): A class template able to perform RAII on any type.
* [ThreadPool](ThreadPool.hpp): Thread pool implementation
* [Timer](Timer.md): Simple timer based on std::chrono.

## Standard library replacements

* [string](string.hpp): fixed-size drop-in replacement for `std::string`
* [vector](vector.hpp): fixed-size drop-in replacement for `std::vector`
* [function](function.hpp): fixed-size drop-in replacement for `std::function`

## Packages

* [Lua](lua/README.md) (enabled with the PUTILS_BUILD_LUA CMake option)
* [Python](python/README.md) (enabled with the PUTILS_BUILD_PYTHON CMake option)
* [Mediator](mediator/README.md) (enabled with the PUTILS_BUILD_MEDIATOR CMake option)
* [Meta](meta/README.md)
* [PluginManager](pluginManager/README.md)
* [PSE](pse/README.md) (enabled with the PUTILS_BUILD_PSE CMake option)
* [Reflection](reflection/README.md)

PSE makes use of [Conan](https://conan.io/) for dependency management. The necessary packages will be automatically downloaded when you run CMake, but Conan must be installed separately by running:
```
pip install conan
```

## Function-only headers

* [angle.hpp](angle.hpp): helper functions when manipulating radians
* [anonymous_variable.hpp](anonymous_variable.hpp): macro to define an anonymous variable
* [casts.hpp](casts.hpp): static and dynamic casts for std::unique_ptr
* [chop.hpp](chop.hpp): function removing any newline and carriage return characters from the end of a string.
* [clamp.hpp](clamp.hpp): function that returns a value clamped between two bounds
* [concat.hpp](concat.hpp): function taking any number of parameters and concatenating them into a string.
* [curl.hpp](curl.hpp): function that returns the content of a `curl` request done over CLI (to avoid having to install curl packages. This is NOT the optimal way to do an HTTP request)
* [erase.hpp](erase.hpp): function taking a container and a value and erasing the value from the container.
* [file_extension.hpp](file_extension.hpp): function returning a file's extension
* [fwd.hpp](fwd.hpp): macro which calls std::forward for the correct type
* [get_current_dir.hpp](get_current_dir.hpp): function which returns the current working directory as a string
* [get_last_error_as_string.hpp](get_last_error_as_string.hpp): function for Windows
* [go_to_bin_dir.hpp](go_to_bin_dir.hpp): function which takes `argv[0]` as parameter and changes the current working directory to that containing the executed file.
* [hash.hpp](hash.hpp): Pre-defined hashers for enums and pairs
* [lengthof.hpp](lengthof.hpp): macro returning the length of an array
* [lock_and_run.hpp](lock_and_run.hpp): function which takes a mutex, a function and its parameters. Creates a scoped lock for the mutex and runs the function.
* [macro_as_string.hpp](macro_as_string.hpp): returns the value of a macro as a string
* [MagicaVoxel.hpp](MagicaVoxel.hpp): structure declarations for the [MagicaVoxel format](https://ephtracy.github.io/)
* [MagicaVoxelToPolyvoxLoader](MagicaVoxelToPolyvoxLoader): conversion functions to parse the MagicaVoxel format into [PolyVox](https://bitbucket.org/volumesoffun/polyvox/src/develop/) objects
* [members.hpp](members.hpp): help with class members
* [rand.hpp](rand.hpp): function using C++ 11's random number generation facilities
* [read_stream.hpp](read_stream.hpp): function which extracts an entire std::istream to a std::string
* [regex.hpp](regex.hpp): utilities to easily construct and match regexes through literal operators
* [remove_if.hpp](remove_if.hpp): function taking a container and a predicate and calling std::remove_if
* [same_sign.hpp](same_sign.hpp): function indicating whether two scalars have the same sign
* [set_thread_name.hpp](set_thread_name.hpp): sets the OS-level name for the current thread
* [sign.hpp](sign.hpp): function returning -1, 0 or 1 depending on the sign of the parameter
* [termcolor.hpp](termcolor.hpp): colors for the terminal. Credit goes to [ikalnytskyi](https://github.com/ikalnytskyi/termcolor)
* [to_string.hpp](to_string.hpp): uses a std::stringstream to serialize an object
* [traits.hpp](traits.hpp): type traits
* [url.hpp](url.hpp): URL encoder and decoder
* [with.hpp](with.hpp): `with` macro mimicking the Kotlin construct

## CMake tools

* [conan](CMakeModules/conan.cmake): lets you run conan commands directly from cmake
* [conan_helper](CMakeModules/conan_helper.cmake): provides a helper macro to simply list conan dependencies
* [cotire](https://github.com/sakra/cotire): lets you easily set up precompiled-header and single-compilation-unit compilations to speed up builds