# [table](table.hpp)

A type-safe association table that maps any key of any type to any value of any type.

### Members

##### make function
```cpp
auto make_table(KeyValues &&...keyValues);
```
Create a table mapping keyValues two by two (see Example).

##### get_value
```cpp
void get_value(auto &&key, auto &&func);
```
Retrieve the value associated to `key` and call `func` by passing it that value.
See `Example`.

##### get_key
```cpp
auto get_key(auto &&value, auto &&func);
```
Retrieve the key associated to `value` and call `func` by passing it that key.
See `Example`.

### Example

```cpp
inline void table()
{
    /*
     * Type map
     */

    auto types = putils::make_table(
            "string", putils::meta::type<std::string>(),
            "int", putils::meta::type<int>(),
            "float", putils::meta::type<float>()
    );

    // Get a key from type
    types.get_key(putils::meta::type<std::string>(),
                  [](auto &&value)
                  {
                      std::cout << value << std::endl;
                  });

    // Get a type from key
    types.get_value("int",
            [](auto &&t)
            {
                using Type = putils_wrapped_type(t);
                std::cout << typeid(Type).name() << std::endl;
            }
    );

    /*
     * Any table
     */

    auto table = putils::make_table(
            "42", 42,
            12, "Hi there"
    );

    table.get_value("42", [](auto &&value) { std::cout << value << std::endl; });
    table.get_key(42, [](auto &&value) { std::cout << value << std::endl; });
    table.get_value(12, [](auto &&value) { std::cout << value << std::endl; });
    table.get_key("Hi there", [](auto &&value) { std::cout << value << std::endl; });
}
```
