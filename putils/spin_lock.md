# [spin_lock](spin_lock.hpp)

Implementation of a spin lock (alternative to a mutex which keeps threads active).

## Members

### lock

```cpp
void lock() noexcept;
```

Takes exclusive ownership of the spin lock. Any successive calls to `lock` will fail until `unlock` is called.

### unlock

```cpp
void unlock() noexcept;
```

Releases exclusive ownership of the spin lock.

### lock_shared

```cpp
void lock_shared() noexcept;
```

Takes shared ownership of the spin lock. Multiple threads can take shared ownership, so long as no thread has exclsuive ownership.

### unlock_shared

```cpp
void unlock_shared() noexcept;
```

Releases shared ownership of the spin lock.

### unlock_and_lock_shared

```cpp
void unlock_and_lock_shared() noexcept;
```

Releases exclusive ownership and takes shared ownership.

## Helper types

### lock

Structure that allows for scope-limited, raii-based exclusive ownership.

#### Members

##### Ctor

```cpp
lock(spin_lock & spin_lock) noexcept;
```

Takes exclusive ownership of `spin_lock`.

##### Dtor

```cpp
~lock() noexcept;
```

Releases ownership.

##### lock

```cpp
void lock() noexcept;
```

Takes ownership again (if it had previously been released).

##### unlock

```cpp
void unlock() noexcept;
```

Releases ownership.

### shared_lock

Structure with the same semantics as `lock`, but for shared ownership.