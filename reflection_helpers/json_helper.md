# [json_helper](json_helper.hpp)

Helper functions for manipulating reflectible types in JSON.

## fromJSON

```cpp
template<typename T>
void fromJSON(const nlohmann::json & object, T && out);
```

For each reflectible attribute in `out`, parses it from `object`.

## toJSON

```cpp
template<typename T>
nlohmann::json toJSON(const T & obj);
```

For each reflectible attribute in `obj`, outputs it into a json object.