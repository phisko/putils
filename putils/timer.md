# [timer](timer.hpp)

Simple timer class based on std::chrono.

## Members

### Constructor

```cpp
timer(const t_duration & duration) noexcept;
```

Sets the time before the timer "beeps".

### is_done

```cpp
bool is_done() const noexcept;
```

Is the timer ringing?

### get_times_done

```cpp
float get_times_done() const noexcept;
```

How many times has the timer ringed?

### get_time_since_start

```cpp
seconds get_time_since_start() const noexcept;
```

How long ago did I start the timer?

### get_time_since_done

```cpp
seconds get_time_since_done() const noexcept;
```

How long ago did the timer beep?

### get_time_left

```cpp
seconds get_time_left() const noexcept;
```

How long until it rings?

### restart

```cpp
void restart() noexcept;
```

### Start getter and setter

```cpp
const t_clock::time_point & get_start() const noexcept;
void set_start(t_clock::time_point start) noexcept;
```

### Duration getter and setter

```cpp
const seconds &get_duration() const noexcept;
void set_duration(const seconds & duration) noexcept;
```

### Helper types

```cpp
using t_duration = std::chrono::duration<double>;
using t_clock = std::chrono::system_clock;
using seconds = t_duration;
```
