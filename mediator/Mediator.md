# [Mediator](Mediator.hpp)

Manages various [Modules](Module.md) and lets them send each other `DataPackets`.

## Members

### addModule

```cpp
void addModule(BaseModule &m);
```

Adds a new module to be managed. Note that the `Mediator` is not responsible for the `Module`'s lifetime.

### removeModule

```cpp
void removeModule(BaseModule &m);
```

Removes a module.

### send

```cpp
void send(T &&data);
```

Sends `data` to all `Modules` "subscribed" to `T` (see [Module](Module.md)).