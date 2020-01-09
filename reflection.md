# [Reflection](reflection.hpp)

A simple, stand-alone, header-only and easily pluggable reflection system.

This does not allow you to introspect any type provided by any library. It simply provides an API that you can implement in your own classes so that you can introspect them.
 
A good example of this is the [Serializable](reflection/Serializable.md) class, that uses reflection to inspect itself and serialize its attributes.

Another example is the [registerType](../lua/README.md) function provided for lua, that inspects a type and registers all its attributes and methods to a lua state.

## Overview

Making a type reflectible is done like so:

```cpp
struct Reflectible {
    int i = 0;

    int getValue() const;

    putils_reflection_class_name(Reflectible);
    putils_reflection_attributes(
        putils_reflection_attribute(&Reflectible::i)
    );
    putils_reflection_methods(
        putils_reflection_attribute(&Reflectible::getValue)
    );
};
```

Accessing a type's name, attributes and methods is done like so:

```cpp
int main() {
    Reflectible obj;

    std::cout << putils::reflection::get_class_name<Reflectible>() << '\n';

    putils::reflection::for_each_attribute<Reflectible>(
        [&](const char * name, auto memberPtr) {
            std::cout << name << ": " << obj.*memberPtr << '\n';
        }
    );

    putils::reflection::for_each_method<Reflectible>(
        [&](const char * name, auto memberPtr) {
            std::cout << name << ": " << (obj.*memberPtr)() << '\n';
        }
    );
    return 0;
}
```

## API

Making a type reflectible consists in implementing (up to) 4 static member functions that provide type information.

### get_class_name

```cpp
static const auto reflection_get_class_name();
```
Returns the class name.
Can be easily generated with `putils_reflection_class_name`.

### get_attributes

```cpp
static const auto & reflection_get_attributes();
```
Returns a [table](https://github.com/phisko/putils/blob/master/meta/table.md) mapping strings to pointers to the attributes.
Can be easily generated with `putils_reflection_attributes`.

### get_methods

```cpp
static const auto & reflection_get_methods();
```
Returns a [table](https://github.com/phisko/putils/blob/master/meta/table.md) mapping strings to pointers to the methods.
Can be easily generated with `putils_reflection_methods`.

### get_parents
```cpp
static const auto & reflection_get_parents();
```
Returns a [table](https://github.com/phisko/putils/blob/master/meta/table.md) mapping strings to `putils::meta::type` objects for each of the class' parents.
Can be easily generated with `putils_reflection_parents`.

## Type traits

These type traits are defined to let client code check whether a given type is reflectible.

```cpp
namespace putils::reflection {
    template<typename T>
    struct has_class_name;
    template<typename T>
    struct has_attributes;
    template<typename T>
    struct has_methods;
    template<typename T>
    struct has_parents;
}
```

## Iterating attributes

Once a type is declared reflectible, iterating over its parents, attributes and methods is made easy by the following helper functions. Note that calling these functions with a non-reflectible type is supported, and will do nothing.

### for_each_attribute

```cpp
namespace putils::reflection {
    template<typename T, typename Func> // Func: void(const char * name, MemberPointer ptr)
    void for_each_attribute(Func && func);
}
```

Lets client code iterate over the attributes for a given type. See the `Example` section below.

### for_each_method

```cpp
namespace putils::reflection {
    template<typename T, typename Func> // Func: void(const char * name, MemberPointer ptr)
    void for_each_method(Func && func);
}
```

Lets client code iterate over the methods for a given type. See the `Example` section below.

### for_each_parent

```cpp
namespace putils::reflection {
    template<typename T, typename Func> // Func: void(const char * name, TypeObject type)
    void for_each_parent(Func && func);
}
```

Lets client code iterate over the parents for a given type. See the `Example` section below.

## Helper macros

### putils_reflection_class_name(ClassName)

Defines a `reflection_get_class_name` static member function returning the macro parameter as a string:

### putils_reflection_attributes(attributes...)

Defines a `reflection_get_attributes` static member function returning a table of attributes.

### putils_reflection_methods(methods...)

Defines a `reflection_get_methods` static member function returning a table of methods.

### putils_reflection_parents(parents...)

Defines a `reflection_get_parents` static member function returning a table of parent types.

### putils_reflection_attribute(memberPtr)

Takes a member pointer as parameter and generates a pair of parameters under the form `"var", &Class::var` to avoid redundancy when passing parameters to `putils::make_table`. For instance:

```cpp
const auto table = putils::make_table(
    "x", &Point::x,
    "y", &Point::y
);
```

can be refactored to:

```cpp
const auto table = putils::make_table(
    putils_reflection_attribute(&Point::x),
    putils_reflection_attribute(&Point::y)
);
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
const auto table = putils::make_table(
    putils_reflection_attribute_private(&Human::_name),
    putils_reflection_attribute_private(&Human::_age)
);
```

### putils_reflection_parent(name)

Provides the same functionality as `putils_reflection_attribute`, but for types. It takes a type name as parameter and generates a pair of parameters under the form `"className", putils::meta::type<className>{}` to avoid redundancy when passing parameters to `putils::make_table`.

## Example

```cpp
class ExampleParent {};

class Test : public ExampleParent {
public:
    std::string exampleMethod() const { return "Method"; }

private:
    std::string _exampleAttribute = "Attribute";

public:
    putils_reflection_class_name(Test);
    putils_reflection_attributes(
        putils_reflection_attribute_private(&Test::_exampleAttribute)
    );
    putils_reflection_methods(
        putils_reflection_attribute(&Test::exampleMethod)
    );
    putils_reflection_parents(
        putils_reflection_parent(ExampleParent)
    );
};

int main() {
    Test t;

    // Walk attributes
    putils::reflection::for_each_attribute<Test>(
        [&](const char * name, auto member) {
            std::cout << name << ": " << t.*ptr << std::endl;
        }
    );

    // Walk methods
    putils::reflection::for_each_method<Test>(
        [&](const char * name, auto member) {
            std::cout << name << ": " << (t.*ptr)() << std::endl;
        }
    );

    // Walk parents
    putils::reflection::for_each_parent<Test>(
        [&](const char * name, auto parent) {
            using ParentType = putils_wrapped_type(parent);
            std::cout << name << ": " << typeid(ParentType).name() << std::endl;
        }
    );

    return 0;
}
```
