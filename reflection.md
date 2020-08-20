# [Reflection](reflection.hpp)

A simple, stand-alone, header-only and easily pluggable reflection system.

This provides an API that you can implement for any type so that you can introspect its attributes, methods, parent types and any types it "uses".

The "used types" concept can refer to anything really, but one interesting use case is when registering a type with scripting languages. If you're going to be accessing a type's attributes through a scripting language, chances are you also want to register those attributes' types.

An example use case for this API is the [registerType](../lua/README.md) function provided for lua, that inspects a type and registers all its attributes and methods to a lua state.

## Overview

Making a type reflectible is done like so:

```cpp
struct Parent {};

struct Reflectible : Parent {
    int i = 0;

    int getValue() const { return i; }
};

#define refltype Reflectible
putils_reflection_info {
    putils_reflection_class_name;
    putils_reflection_attributes(
        putils_reflection_attribute(i)
    );
    putils_reflection_methods(
        putils_reflection_attribute(getValue)
    );
    putils_reflection_parents(
        putils_reflection_type(Parent)
    );
    putils_reflection_used_types(
        putils_reflection_type(int)
    );
};
#undef refltype
```

Note that all the information does not need to be present. For instance, for a type with only two attributes, and for which we don't want to expose the class name:

```cpp
struct Simple {
    int i = 0;
    double d = 0;
};

#define refltype Simple
putils_reflection_info {
    putils_reflection_attributes(
        putils_reflection_attribute(i),
        putils_reflection_attribute(d)
    );
};
#undef refltype
```

Accessing a type's name, attributes, methods and used types is done like so:

```cpp
int main() {
    Reflectible obj;

    std::cout << putils::reflection::get_class_name<Reflectible>() << '\n';

    // Obtaining member pointers
    {
        putils::reflection::for_each_attribute<Reflectible>(
            [&](const char * name, auto memberPtr) {
                assert(memberPtr == &Reflectible::i);
                std::cout << name << ": " << obj.*memberPtr << '\n';
            }
        );
        constexpr auto memberPtr = putils::reflection::get_attribute<int, Reflectible>("i");
        static_assert(memberPtr == &Reflectible::i);
    }

    // Obtaining attributes of a specific object
    {
        putils::reflection::for_each_attribute(obj,
            [&](const char * name, const auto & attr) {
                assert(&attr == &obj.i);
                std::cout << name << ": " << attr << '\n';
            }
        );
        const auto attr = putils::reflection::get_attribute<int>(obj, "i");
        assert(attr == &obj.i);
    }

    // Obtaining member function pointers
    putils::reflection::for_each_method<Reflectible>(
        [&](const char * name, auto memberPtr) {
            assert(memberPtr == &Reflectible::getValue);
            std::cout << name << ": " << (obj.*memberPtr)() << '\n';
        }
    );

    // Obtaining functors to call the method on a given object
    putils::reflection::for_each_method(obj,
        [](const char * name, auto && func) {
            // func is a functor that calls obj.getValue()
            std::cout << name << ": " << func() << '\n';
        }
    );

    putils::reflection::for_each_parent<Reflectible>(
        [](const auto type) {
            // type: putils::meta::type<Parent>
            using T = putils_wrapped_type(type);
            std::cout << typeid(T).name() << '\n';
        }
    );

    putils::reflection::for_each_used_type<Reflectible>(
        [](const auto type) {
            // type: putils::meta::type<int>
            using T = putils_wrapped_type(type);
            std::cout << typeid(T).name() << '\n';
        }
    );

    return 0;
}
```

All these operations can also be done at compile-time:

```cpp
constexpr Reflectible obj;

constexpr auto & attributes = putils::reflection::get_attributes<Reflectible>();
using ExpectedType = std::tuple<std::pair<const char *, int Reflectible:: *>>;
static_assert(std::is_same_v<std::decay_t<decltype(attributes)>, ExpectedType>);
static_assert(std::get<0>(attributes).second == &Reflectible::i);

constexpr auto attr = putils::reflection::get_attribute<int>(obj, "i");
static_assert(attr == &obj.i);
static_assert(*attr == 0);

constexpr size_t countAttributes() {
    size_t ret = 0;
    putils::reflection::for_each_attribute<Reflectible>(
        [&](const char * name, const auto member) {
            ++ret;
        }
	);
    return ret;
}

static_assert(countAttributes() == 1);
```

Functions are also provided to check if a type exposes a given property:

```cpp
static_assert(putils::reflection::has_class_name<Reflectible>());
static_assert(putils::reflection::has_attributes<Reflectible>());
static_assert(putils::reflection::has_methods<Reflectible>());
static_assert(putils::reflection::has_parents<Reflectible>());
static_assert(putils::reflection::has_used_types<Reflectible>());
```

## API

Making a type reflectible consists in specializing the `putils::reflection::type_info` template with (up to) 5 static members that provide type information.

```cpp
namespace putils::reflection {
	template<typename T>
	struct type_info {
		static constexpr auto class_name = const char *;
		static constexpr auto attributes = std::tuple<std::pair<const char *, MemberPointer>...>;
		static constexpr auto methods = std::tuple<std::pair<const char *, MemberPointer>...>;
		static constexpr auto parents = std::tuple<putils::meta::type<Parent>...>;
		static constexpr auto used_types = std::tuple<putils::meta::type<UsedType>...>;
	};
}
```

For instance, for the `Reflectible` struct given as an example above:

```cpp
template<>
struct putils::reflection::type_info<Reflectible> {
    static constexpr auto class_name = "Reflectible";
    static constexpr auto attributes = std::make_tuple(
        std::make_pair("i", &Reflectible::i)
    );
    static constexpr auto methods = std::make_tuple(
        std::make_pair("getValue", &Reflectible::getValue)
    );
    static constexpr auto parents = std::make_tuple(
        putils::meta::type<Parent>{}
    );
    static constexpr auto used_types = std::make_tuple(
        putils::meta::type<int>{}
    );
};
```

The `type_info` specialization can be easily defined through the use of helper macros, described below.

### class_name

```cpp
static constexpr auto class_name = "MyClass";
```
Can be easily generated with `putils_reflection_class_name`.

### attributes

```cpp
static constexpr auto attributes = std::make_tuple(
    std::make_pair("attribute", &MyClass::attribute),
    ...
);
```
[table](https://github.com/phisko/putils/blob/master/meta/table.md) mapping strings to pointers to the attributes.
Can be easily generated with `putils_reflection_attributes`.

### methods

```cpp
static constexpr auto methods = std::make_tuple(
    std::make_pair("method", &MyClass::method),
    ...
);
```
[table](https://github.com/phisko/putils/blob/master/meta/table.md) mapping strings to pointers to the methods.
Can be easily generated with `putils_reflection_methods`.

### parents
```cpp
static constexpr auto parents = std::make_tuple(
    putils::meta::type<Parent>{},
    ...
);
```
`std::tuple` of `putils::meta::type` objects for each of the type's parents.
Can be easily generated with `putils_reflection_parents`.

### used_types
```cpp
static constexpr auto used_types = std::make_tuple(
    putils::meta::type<UsedType>{},
    ...
);
```
`std::tuple` of `putils::meta::type` objects for each type used by the class (which should also be registered with scripting systems, for instance).
Can be easily generated with `putils_reflection_used_types`.

## Detector functions

The following functions are defined to let client code check whether a given type is reflectible.

```cpp
namespace putils::reflection {
    template<typename T>
    constexpr bool has_class_name();

    template<typename T>
    constexpr bool has_attributes();

    template<typename T>
    constexpr bool has_methods();

    template<typename T>
    constexpr bool has_parents();

    template<typename T>
    constexpr bool has_used_types();
}
```

## Iterating attributes

Once a type is declared reflectible, iterating over any of its reflectible properties is made easy by the following helper functions. Note that calling these functions with a non-reflectible type is supported, and will do nothing.

### for_each_attribute

```cpp
namespace putils::reflection {
    template<typename T, typename Func> // Func: void(const char * name, MemberPointer ptr)
    void for_each_attribute(Func && func);

    template<typename T, typename Func> // Func: void(const char * name, Member && member)
    void for_each_attribute(T && obj, Func && func);
}
```

Lets client code iterate over the attributes for a given type.

### for_each_method

```cpp
namespace putils::reflection {
    template<typename T, typename Func> // Func: void(const char * name, MemberPointer ptr)
    void for_each_method(Func && func);

    template<typename T, typename Func> // Func: void(const char * name, const Functor & functor)
}
```

Lets client code iterate over the methods for a given type.

### for_each_parent

```cpp
namespace putils::reflection {
    template<typename T, typename Func> // Func: void(const char * name, TypeObject type)
    void for_each_parent(Func && func);
}
```

Lets client code iterate over the parents for a given type.

### for_each_used_type

```cpp
namespace putils::reflection {
    template<typename T, typename Func> // Func: void(const char * name, TypeObject type)
    void for_each_used_type(Func && func);
}
```

Lets client code iterate over the types used by a given type.

## Getting specific attributes

### get_attribute

```cpp
template<typename Member, typename T>
std::optional<Member T::*> get_attribute(std::string_view name);

template<typename Member, typename T>
Member * get_attribute(T && obj, std::string_view name);
```

Returns the attribute called `name` if there is one.
* The first overload returns an `std::optional` member pointer (or `std::nullopt`)
* The second overload returns a pointer to `obj`'s attribute (or `nullptr`)

### get_method

```cpp
template<typename Signature, typename T>
std::optional<Signature T::*> get_method(std::string_view name);

template<typename Signature, typename T>
std::optional<Functor> get_method(T && obj, std::string_view name);
```

Returns the method called `name` if there is one.
* The first overload returns an `std::optional` member pointer (or `std::nullopt`)
* The second overload returns an `std::optional` functor which calls the method on `obj` (or `std::nullopt`)

## Helper macros

The following macros can be used to greatly simplify defining the `putils::reflection::type_info` specialization for a type.

These macros expect a `refltype` macro to be defined for the given type:
```cpp
#define refltype ReflectibleType
...
#undef refltype
```

### putils_reflection_info

Declares a specialization of `putils::reflection::type_info` for `refltype`.

### putils_reflection_info_template

Declares a specialization of `putils::reflection_type_info` for a template type, e.g.:
```cpp
template<typename T>
struct MyType {};

template<typename T>
#define refltype MyType<T>
putils_reflection_info_template {
    ...
};
#undef refltype
```

### putils_reflection_friend(type)

Used inside a reflectible type to mark the corresponding `type_info` as `friend`, in order to reflect private fields. Takes as parameter the name of the type.

### putils_reflection_class_name

Defines a `class_name` static string with `refltype` as its value.

### putils_reflection_custom_class_name(name)

Defines a `class_name` static string with the macro parameter as its value.

### putils_reflection_attributes(attributes...)

Defines an `attributes` static table of `std::pair<const char *, MemberPointer>`.

### putils_reflection_methods(methods...)

Defines a `methods` static table of `std::pair<const char *, MemberPointer>`.

### putils_reflection_parents(parents...)

Defines a `parents` static tuple of `putils::meta::type`.

### putils_reflection_attribute(attributeName)

Takes the name of an attribute as parameter and generates of pair of parameters under the form `"var", &refltype::var` to avoid redundancy when passing parameters to `putils::make_table`. For instance:

```cpp
const auto table = putils::make_table(
    "x", &Point::x,
    "y", &Point::y
);
```

can be refactored to:

```cpp
#define refltype Point
const auto table = putils::make_table(
    putils_reflection_attribute(x),
    putils_reflection_attribute(y)
);
#undef refltype
```

### putils_reflection_attribute_private(memberPtr)

Provides the same functionality as `putils_reflection_attribute`, but skips the first character of the attribute's name (such as an `_` or `m`) that would mark a private member. For instance:

```cpp
const auto table = putils::make_table(
    "name", &Human::_name,
    "age", &Human::_age
);
```

can be refactored to:

```cpp
#define refltype Human
const auto table = putils::make_table(
    putils_reflection_attribute_private(_name),
    putils_reflection_attribute_private(_age)
);
#undef refltype
```

### putils_reflection_type(name)

Provides the same functionality as `putils_reflection_attribute`, but for types. It takes a type name as parameter and expands to `putils::meta::type<className>{}` to avoid redundancy when passing parameters to `putils::make_table`.