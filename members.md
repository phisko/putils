# [members](members.hpp)

Type traits and functions for attribute introspection.

## MemberType

```cpp
template<typename MemberPtr>
using MemberType = ...;
```

Alias for the concrete type of a member pointer. For instance:

```cpp
struct A {
    int i = 0;
};
```

`MemberType<decltype(&A::i)>` will be `int`.

## ObjectType

```cpp
template<typename MemberPtr>
using ObjectType = typename detail::ObjectTypeWrapper<std::decay_t<MemberPtr>>::type;
```

Alias for the "parent" type of a member pointer. For instance:

```cpp
struct A {
    int i = 0;
};
```

`ObjectType<decltype(&A::i)>` will be `A`.

## member_offset

```cpp
template<typename T, typename U>
constexpr std::ptrdiff_t member_offset(U T::* member) noexcept;
```

Converts a member pointer to a pointer offset.