# putils
Phiste's C++ Utilities


### Stand-alone utility classes

* [AbstractFactory](AbstractFactory.md) Base class for a factory able to create any type in a given typelist calling ``make<T>()``.
* [CellularAutomaton](CellularAutomaton.md) Simple cellular automaton implementation
* [CLI](CLI.md) Simple and extensible Command-Line Interface.
* [Directory](Directory.md) An abstraction to Unix and Windows directories, which lets you iterate through a directory and get information about each file.
* [JSON](json.md) JSON lexer and pretty-printer
* [NamedPipe](NamedPipe.md) Abstraction to named pipes
* [Observable](Observable.md) A minimalistic (but greatly sufficient) implementation of the Observer pattern.
* [OperatorForward](OperatorForwarder.md) Mix-in class which lets you forward all operators to a given member.
* [Point](Point.md) Simple abstraction to a 2D or 3D point/rectangle.
* [Process](Process.md) Abstraction to a process (useful to run system commands and send them input/get their output)
* [RAII](RAII.md) A class template able to perform RAII on any type.
* [ThreadPool](ThreadPool.hpp) Thread pool implementation
* [Timer](Timer.md) Simple timer based on std::chrono.

### Standard library replacements

* [string](string.hpp): fixed-size drop-in replacement for `std::string`
* [vector](vector.hpp): fixed-size drop-in replacement for `std::vector`
* [function](function.hpp): fixed-size drop-in replacement for `std::function`

### Packages

* [Lua](lua/README.md) (enabled with the PUTILS_BUILD_LUA CMake option)
* [Python](python/README.md) (enabled with the PUTILS_BUILD_PYTHON CMake option)
* [Mediator](mediator/README.md) (enabled with the PUTILS_BUILD_MEDIATOR CMake option)
* [Meta](meta/README.md)
* [PluginManager](pluginManager/README.md)
* [PSE](pse/README.md) (enabled with the PUTILS_BUILD_PSE CMake option)
* [Reflection](reflection/README.md)

### Function-only headers

* [anonymous_variable.hpp](anonymous_variable.hpp): macro to define an anonymous variable
* [casts.hpp](casts.hpp): static and dynamic casts for std::unique_ptr
* [chop.hpp](chop.hpp): function removing any newline and carriage return characters from the end of a string.
* [concat.hpp](concat.hpp): function taking any number of parameters and concatenating them into a string.
* [erase.hpp](erase.hpp): function taking a container and a value and erasing the value from the container.
* [file_extension.hpp](file_extension.hpp): function returning a file's extension
* [fwd.hpp](fwd.hpp): macro which calls std::forward for the correct type
* [get_current_dir.hpp](get_current_dir.hpp): function which returns the current working directory as a string
* [get_last_error_as_string.hpp](get_last_error_as_string.hpp): function for Windows
* [go_to_bin_dir.hpp](go_to_bin_dir.hpp): function which takes `argv[0]` as parameter and changes the current working directory to that containing the executed file.
* [hash.hpp](hash.hpp): Pre-defined hashers for enums and pairs
* [lengthof.hpp](lengthof.hpp): macro returning the length of an array
* [lock_and_run.hpp](lock_and_run.hpp): function which takes a mutex, a function and its parameters. Creates a scoped lock for the mutex and runs the function.
* [members.hpp](members.hpp): help with class members
* [rand.hpp](rand.hpp): function using C++ 11's random number generation facilities
* [read_stream.hpp](read_stream.hpp): function which extracts an entire std::istream to a std::string
* [regex.hpp](regex.hpp): utilities to easily construct and match regexes through literal operators
* [remove_if.hpp](remove_if.hpp): function taking a container and a predicate and calling std::remove_if
* [same_sign.hpp](same_sign.hpp): function indicating whether two scalars have the same sign
* [sign.hpp](sign.hpp): function returning -1, 0 or 1 depending on the sign of the parameter
* [to_string.hpp](to_string.hpp): uses a std::stringstream to serialize an object
* [traits.hpp](traits.hpp): type traits
* [url.hpp](url.hpp): URL encoder and decoder
* [with.hpp](with.hpp): `with` macro mimicking the Kotlin construct
