# [default_constructors](default_constructors.hpp)

Macros to reduce boilerplate code when needing to explicitly default or delete constructors.

## Members

### `PUTILS_MOVE_ONLY`

```cpp
#define PUTILS_MOVE_ONLY(T) \
	T() noexcept = default; \
	PUTILS_DELETE_COPY(T) \
	PUTILS_DEFAULT_MOVE(T)
```

Marks `T` as move-only, with a default ctor, default move constructor and assignment operator, and deleted copy constructor and assignment operator.

### `PUTILS_DEFAULT_COPY`

```cpp
#define PUTILS_DEFAULT_COPY(T) \
	T(const T &) noexcept = default; \
	T & operator=(const T &) noexcept = default;
```

Defaults the copy constructor and assignment operator for `T`.

### `PUTILS_DELETE_COPY`

```cpp
#define PUTILS_DELETE_COPY(T) \
	T(const T &) noexcept = delete; \
	T & operator=(const T &) noexcept = delete;
```

Deletes the copy constructor and assignment operator for `T`.

### `PUTILS_DEFAULT_MOVE`

```cpp
#define PUTILS_DEFAULT_MOVE(T) \
	T(T &&) noexcept = default; \
	T & operator=(T &&) noexcept = default;
```

Defaults the move constructor and assignment operator for `T`.

### `PUTILS_DELETE_MOVE`

```cpp
#define PUTILS_DELETE_MOVE(T) \
	T(T &&) noexcept = delete; \
	T & operator=(T &&) noexcept = delete;
```

Deletes the move constructor and assignment operator for `T`.

### `PUTILS_DELETE_COPY_AND_MOVE`

```cpp
#define PUTILS_DELETE_COPY_AND_MOVE(T) \
	PUTILS_DELETE_COPY(T) \
	PUTILS_DELETE_MOVE(T)
```

Deletes the copy and move constructors and assignment operators for `T`.