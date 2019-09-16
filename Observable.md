# [Observable<Args...>](Observable.hpp)

A minimalistic (but greatly sufficient) implementation of the Observer pattern.

An observer is simply defined as an `std::function<void(const Args... &)>`, meaning an observer can be a simple lambda which captures any needed context.

Classes inheriting from `Observable` can notify their observers by calling `changed(const Args... &)`.

`Args` can be used to notify observers about one property in particular (perhaps through an enum indicating which member changed)

### Members

##### addObserver
```cpp
intptr_t addObserver(const std::function<void(const Args &...)> &observer);
Observable &operator+=(const std::function<void()> &observer) noexcept { addObserver(observer); return *this; }
```
Returns an ID for the observer, letting you remove it later.

### removeObserver
```cpp
void removeObserver(intptr_t id);
```
Removes the observer with the given `id`.

### changed
```cpp
void changed(const Args &...args);
void operator()(const Args &...args);
```
Notifies observers that the observable has changed.
