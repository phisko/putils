# [ThreadPool](ThreadPool.hpp)

Implementation of a thread pool capable of running asynchronous tasks.

## Members

### Ctor

```cpp
ThreadPool(size_t threads) noexcept;
```

Starts the given number of worker threads, that will wait until tasks are queued.

### runTask

```cpp
void runTask(function && f) noexcept;
```

Queues a task to be run by a worker thread.

### getThreadCount

```cpp
size_t getThreadCount() const noexcept;
```

Returns the number of worker threads.