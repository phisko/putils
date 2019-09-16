# [Mediator](Mediator.hpp)

Manages various [Modules](Module.md) and lets them send each other `DataPackets`.

Also lets `Modules` run asynchronous tasks.

### Members

##### addModule

```cpp
void addModule(BaseModule &m);
```

Adds a new module to be managed. Note that the `Mediator` is not responsible for the `Module`'s lifetime.

##### removeModule

```cpp
void removeModule(BaseModule &m);
```

Removes a module.

##### send

```cpp
void send(T &&data);
```

Sends `data` to all `Modules` "subscribed" to `T` (see [Module](Module.md)).

##### runTask

```cpp
void runTask(const std::function<void()> &f);
```

Runs `f` asyncrhonously.

##### completeTasks

```cpp
void completeTasks();
```

Wait for all current tasks to finish.

##### running

```cpp
std::atomic<bool> running { true };
```

Gives `Modules` a common value to check to make sure the program is still running.
