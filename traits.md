# [traits](traits.hpp)

Type traits.

## is_streamable

```cpp
template<typename S, typename T>
using is_streamable = ...;
```

Indicates whether `T` can be streamed into `S`.

## is_unstreamable

```cpp
template<typename S, typename T>
using is_unstreamable = ...;
```

Indicates whether `T` can be streamed out of `S`.

## is_iterable

```cpp
template <typename T>
using is_iterable = ...;
```

Indicates whether T can be iterated over.

## is_function

```cpp
template<typename T>
using is_function = ...;
```

Indicates whether `T` is a function pointer, an `std::function` or a `putils::function`.

## function_signature

```cpp
template<typename T>
using function_signature = ...;
```

Extracts the function signature of `T`.

## function_return_type

```cpp
template<typename T>
using function_return_type = ...;
```

Extracts the return type of `T`.

## function_arguments 

```cpp
template<typename T>
using function_arguments = ...;
```

Extracts the arguments for `T` as an `std::tuple<Args...>`.

## is_member_function_ptr

```cpp
template<typename T>
using is_member_function_ptr = ...;
```

Indicates whether `T` is a pointer to a member function.

## member_function_decay

```cpp
template<typename T>
using member_function_decay = ...;
```

Removes `const` and `noexcept` from the member function signature.

## member_function_signature 

```cpp
template<typename T>
using member_function_signature = ...;
```

Extracts the function signature of `T`.

## member_function_return_type 

```cpp
template<typename T>
using member_function_return_type = ...;
```

Extracts the return type of `T`.

## member_function_arguments 

```cpp
template<typename T>
using member_function_arguments = ...;
```

Extracts the arguments for `T` as an `std::tuple<Args...>`.

## is_specialization

```cpp
template<typename T, template<typename...> class Template>
struct is_specialization;
```

Indicates whether `T` is a specialization of `Template`.

## Member detector

```cpp
#define putils_member_detector(member) ...
```

Defines a
```cpp
template<typename T>
struct has_member_##member;
```
type that indicates whether `T` has an attribute named `member`.

## Nested type detector

```cpp
#define putils_nested_type_detector(member) ...
```

Has the same semantics as `putils_member_detector`, but for nested types.