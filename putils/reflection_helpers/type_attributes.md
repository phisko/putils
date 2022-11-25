# [type_attributes](type_attributes.hpp)

Map of runtime attributes for a given type.

## type_attributes

```cpp
struct type_attributes {
	std::unordered_map<std::string, const attribute_info *> map;
};
```

Holds a description of each of a type's attributes.

## attribute_info

```cpp
struct attribute_info {
	std::ptrdiff_t offset;
	size_t size;
	type_attributes attributes;
	std::optional<array_helper> array;
	std::optional<map_helper> map;
};
```

Information about a single attribute.

### Members

* `offset`: offset from the parent object's base address
* `size`: size of the attribute in bytes
* `attributes`: map of this attribute's attributes
* `array`: if this is an array, helper functions to access it
* `map`: if this is a map, helper functions to access it

### array_helper

```cpp
struct array_helper {
	std::function<size_t(void * attribute)> get_size = nullptr;
	std::function<void *(void * attribute, size_t index)> get_element = nullptr;
	
	using iterator = std::function<void(void * element)>;
	std::function<void(const iterator & callback)> for_each = nullptr;

	type_attributes element_attributes;
};
```

Structure of helper functions to access an array attribute.

#### Members

* `get_size`: returns the number of elements
* `get_element`: returns a pointer to the element at `index`
* `for_each`: calls `callback` for each element
* `element_attributes`: attributes of the element type

### map_helper

```cpp
struct map_helper {
	std::function<size_t(void * attribute)> get_size = nullptr;
	std::function<void *(void * attribute, const char * key_string)> get_value = nullptr;

	using iterator = std::function<void(const void * key, void * value)>;
	std::function<void(const iterator & callback)> for_each = nullptr;

	type_attributes key_attributes;
	type_attributes value_attributes;
};
```

Structure of helper functions to access a map attribute.

#### Members

* `get_size`: returns the number of elements
* `get_value`: returns a pointer to the value for `key_string` (where `key_string` is the serialized key)
* `for_each`: calls `callback` for each element
* `key_attributes`: attributes of the key type
* `value_attributes`: attributes of the value type