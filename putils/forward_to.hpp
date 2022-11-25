#pragma once

#include "putils/meta/fwd.hpp"

// Call callable (with no capture)
#define putils_forward_to(callable) [](auto && ... args) noexcept { return callable(FWD(args)...); }

// Capture object by reference and call function on it
#define putils_forward_to_ref(object, function) [&object](auto && ... args) noexcept { return object.function(FWD(args)...); }

// Capture object by value and call function on it
#define putils_forward_to_ptr(object, function) [object](auto && ... args) noexcept { return object->function(FWD(args)...); }

// Capture this and call function on it
#define putils_forward_to_this(function) putils_forward_to_ptr(this, function)
