# [json_helper](json_helper.hpp)

Helper functions for manipulating [reflectible](https://github.com/phisko/reflection) types in JSON.

## from_json

```cpp
template<typename T>
void from_json(const nlohmann::json & object, T && out);
```

Parses `out` from `object`.

## to_json

```cpp
template<typename T>
nlohmann::json to_json(const T & obj);
```

Outputs `obj` into a JSON object.