# [RAII](RAII.hpp)

A class template able to perform RAII on any type. `RAII<T>` is constructed with a `T` and an `std::function<void(T)>` that will be called at destruction time.
Automatic conversions from `RAII<T>` to `T` exist to greatly simplify usage.

Movable, but not copyable.

### Members

##### Constructor

```cpp
RAII(T &&res, std::function<void(T &)> &&dtor = [](T &) {});
```

Takes the ressource to manage, and the `dtor` function used to release it.

##### Operators and get

Operators are in place for implicit conversion to the contained resource.

A `get()` function also lets you explicitly access the resource.

### Example

For instance, to securely use a C socket:

```cpp
auto socket = RAII<int>{ socket(...), [](int sock) { close(sock); } };
```

For an OpenSSL object:

```cpp
auto ctx = RAII<SSL*> { nullptr, [](SSL *ssl) { SSL_free(ssl); } };
```
