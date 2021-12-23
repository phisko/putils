# [Attributes](Attributes.hpp)

Map of runtime attributes for a given type.

## AttributeInfo

```cpp
struct AttributeInfo {
	std::ptrdiff_t offset;
	size_t size;
	AttributeMap attributes;
	std::optional<ArrayHelper> arrayHelper;
	std::optional<MapHelper> mapHelper;
};
```

Information about a single attribute.

### Members

* `offset`: offset from the parent object's base address
* `size`: size of the attribute in bytes
* `attributes`: map of this attribute's attributes
* `arrayHelper`: if this is an array, helper functions to access it
* `mapHelper`: if this is a map, helper functions to access it

### ArrayHelper

```cpp
struct ArrayHelper {
	std::function<size_t(void * attribute)> getSize = nullptr;
	std::function<void *(void * attribute, size_t index)> getElement = nullptr;
	
	// using Iterator = putils::function<void(void * element), KENGINE_META_ATTRIBUTES_ITERATOR_FUNCTION_SIZE>;
	using Iterator = std::function<void(void * element)>;
	std::function<void(const Iterator & callback)> forEach = nullptr;

	AttributeMap elementAttributes;
};
```

Structure of helper functions to access an array attribute.

#### Members

* `getSize`: returns the number of elements
* `getElement`: returns a pointer to the element at `index`
* `forEach`: calls `callback` for each element
* `elementAttributes`: attributes of the element type

### MapHelper

```cpp
struct MapHelper {
	std::function<size_t(void * attribute)> getSize = nullptr;
	std::function<void *(void * attribute, const char * keyString)> getValue = nullptr;

	// using Iterator = putils::function<void(const void * key, void * value), KENGINE_META_ATTRIBUTES_ITERATOR_FUNCTION_SIZE>;
	using Iterator = std::function<void(const void * key, void * value)>;
	std::function<void(const Iterator & callback)> forEach = nullptr;

	AttributeMap keyAttributes;
	AttributeMap valueAttributes;
};
```

Structure of helper functions to access a map attribute.

#### Members

* `getSize`: returns the number of elements
* `getValue`: returns a pointer to the value for `keyString` (where `keyString` is the serialized key)
* `forEach`: calls `callback` for each element
* `keyAttributes`: attributes of the key type
* `valueAttributes`: attributes of the value type