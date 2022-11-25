# [observable<Args...>](observable.hpp)

A minimalistic (but greatly sufficient) implementation of the observer pattern.

An observer is simply defined as an `std::function<void(const Args... &)>`, meaning an observer can be a simple lambda which captures any needed context.

Classes inheriting from `observable` can notify their observers by calling `changed(const Args... &)`.

`Args` can be used to notify observers about one property in particular (perhaps through an enum indicating which member changed)

## Members

### add_observer
```cpp
intptr_t add_observer(const std::function<void(const Args &...)> &observer) noexcept;
observable &operator+=(const std::function<void()> &observer) noexcept;
```
Returns an ID for the observer, letting you remove it later.

### remove_observer
```cpp
void remove_observer(intptr_t id) noexcept;
```
Removes the observer with the given `id`.

### changed
```cpp
void changed(const Args &...args) noexcept;
void operator()(const Args &...args) noexcept;
```
Notifies observers that the observable has changed.
