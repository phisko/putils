# [type](type.hpp)

Object representation of a type.

A `pmeta::type` is an empty object which can be passed to template functions to simply identify a type.

### Static members

##### wrapped
```cpp
using wrapped = T;
```
Provides access to the actual type this object represents.

##### index
```cpp
using type_index = std::size_t;
static const type_index index;
```
Unique index associated to the type, calculated only once at program startup. Very useful to map types to various objects.

For instance, in an Entity Component System, `Entities` may map their `Components` to their type, and retrieve them using it.

### Macros

```cpp
#define pmeta_typeof(object) std::decay_t<decltype(object)>
#define pmeta_wrapped(typeObject) typename pmeta_typeof(typeObject)::wrapped
```
These macros are defined to make using `pmeta::type` objects as `auto` parameters easier.

