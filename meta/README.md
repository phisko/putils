# Meta

Phiste's metaprogramming utilities.

### Classes

* [GenLinearHierarchy](GenLinearHierarchy.md): given a class template `T` and a typelist `List`, generate a new type inheriting from `T` specialized on each type in `List`.
* [type](type.md): object representation of a type
* [table](table.md): map any number of keys of any number of types to any number of values of any number of types. All with compile-time type safety.

### Function-only headers

* apply.hpp: C++14 implementation of `std::apply`
* for_each.hpp: call a function for each element in a tuple, or instantiate a template type and call a static function for each type in a list
* has_type.hpp: type_trait indicating whether a tuple contains a certain type
* new_from_tuple.hpp: a make_from_tuple that creates a heap-based object
* nameof.hpp: helper macros to get a class/variable name and avoid matching raw strings to actual names
