# [forward_to](forward_to.hpp)

Helper macros to declare lambdas that simply forward arguments to an object.

## Members

### putils_forward_to(callable)

Simply calls `callable` (with no capture). Useful for calling a method on a global object.

### putils_forward_to_ref(object, function)

Captures `object` by reference and calls `object.function()`.

### putils_forward_to_ptr(object, function)

Captures `object` by value (assuming it's a pointer) and calls `object->function()`.

### putils_forward_to_this(function)

Captures `this` by value and calls `this->function()`.