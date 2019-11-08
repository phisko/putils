# [Reflection](reflection.hpp)

### Reflection methods

##### get_class_name

```cpp
static const auto get_class_name();
```
Returns the class name.
Can be easily generated with `pmeta_get_class_name`.

##### get_attributes

```cpp
static const auto &get_attributes();
```
Returns a [table](https://github.com/phiste/putils/blob/master/meta/table.md) mapping strings to pointers to the attributes.
Can be easily generated with `pmeta_get_attributes`.

##### get_methods

```cpp
static const auto &get_methods();
```
Returns a [table](https://github.com/phiste/putils/blob/master/meta/table.md) mapping strings to pointers to the methods.
Can be easily generated with `pmeta_get_methods`.

##### get_parents
```cpp
static const auto &get_parents();
```
Returns a [table](https://github.com/phiste/putils/blob/master/meta/table.md) mapping strings to `pmeta::type` objects for each of the class' parents.
Can be easily generated with `pmeta_get_parents`.

### Type traits

```cpp
template<typename T>
struct has_member_get_class_name;
template<typename T>
struct has_member_get_attributes;
template<typename T>
struct has_member_get_methods;
template<typename T>
struct has_member_get_parents;
```

Let client code check whether a given-type is reflectible.

### Iterating attributes

##### for_each_attribute

```cpp
template<typename T, typename Func> // Func: void(const char * name, MemberPointer ptr)
void for_each_attribute(Func && func);
```

Lets client code iterate over the attributes for a given type. See the `Example` section below.

##### for_each_method

```cpp
template<typename T, typename Func> // Func: void(const char * name, MemberPointer ptr)
void for_each_method(Func && func);
```

Lets client code iterate over the methods for a given type. See the `Example` section below.

##### for_each_parent

```cpp
template<typename T, typename Func> // Func: void(const char * name, TypeObject type)
void for_each_parent(Func && func);
```

Lets client code iterate over the parents for a given type. See the `Example` section below.

### Helper macros

##### pmeta_reflectible_attribute(memberPtr)

Takes a member pointer as parameter and generates a pair of parameters under the form `"var", &Class::var` to avoid redundancy when passing parameters to `pmeta::make_table`. For instance:

```cpp
const auto table = pmeta::make_table(
    "x", &Point::x,
    "y", &Point::y
);
```

can be refactored to:

```cpp
const auto table = pmeta::make_table(
    pmeta_reflectible_attribute(&Point::x),
    pmeta_reflectible_attribute(&Point::y)
);
```

##### pmeta_reflectible_attribute_private(memberPtr)

Provides the same functionality as `pmeta_reflectible_attribute`, but skips the first character of the attribute's name (such as an `_` or `m`) that would mark a private member. For instance:

```cpp
const auto table = pmeta::make_table(
    "name", &Human::_name,
    "age", &Human::_age
);
```

can be refactored to:

```cpp
const auto table = pmeta::make_table(
    pmeta_reflectible_attribute_private(&Human::_name),
    pmeta_reflectible_attribute_private(&Human::_age)
);
```

##### pmeta_get_class_name(ClassName)

Defines a `get_class_name` static member function returning the macro parameter as a string:

##### pmeta_get_attributes(attributes...)

Defines a `get_attributes` static member function returning a table of attributes.

##### pmeta_get_methods(methods...)

Defines a `get_methods` static member function returning a table of methods.

##### pmeta_get_parents(ParentClassNames...)

Defines a `get_parents` static member function returning a table of parent types.

### Example

```cpp
class ExampleParent {};

class Test : public ExampleParent {
public:
    std::string exampleMethod() const { return "Method"; }

private:
    std::string _exampleAttribute = "Attribute";

public:
    pmeta_get_class_name(Test);
    pmeta_get_attributes(
        pmeta_reflectible_attribute_private(&Test::_exampleAttribute)
    );
    pmeta_get_methods(
        pmeta_reflectible_attribute(&Test::exampleMethod)
    );
    pmeta_get_parents(
        pmeta_reflectible_parent(ExampleParent)
    );
};

int main() {
    Test t;

    // Walk attributes
    putils::for_each_attribute<Test>(
        [&](const char * name, auto member) {
            std::cout << name << ": " << t.*ptr << std::endl;
        }
    );

    // Walk methods
    putils::for_each_method<Test>(
        [&](const char * name, auto member) {
            std::cout << name << ": " << (t.*ptr)() << std::endl;
        }
    );

    // Walk parents
    putils::for_each_parent<Test>(
        [&](const char * name, auto parent) {
            using ParentType = pmeta_wrapped(parent);
            std::cout << name << ": " << typeid(ParentType).name() << std::endl;
        }
    );

    return 0;
}
```
