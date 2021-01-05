# [Timer](Timer.hpp)

Simple timer class based on std::chrono.

## Members

### Constructor

```cpp
Timer(const t_duration & duration) noexcept;
```

Sets the time before the timer "beeps".

### isDone

```cpp
bool isDone() const noexcept;
```

Is the timer ringing?

### getTimesDone

```cpp
float getTimesDone() const noexcept;
```

How many times has the timer ringed?

### getTimeSinceStart

```cpp
seconds getTimeSinceStart() const noexcept;
```

How long ago did I start the timer?

### getTimeSinceDone

```cpp
seconds getTimeSinceDone() const noexcept;
```

How long ago did the timer beep?

### getTimeLeft

```cpp
seconds getTimeLeft() const noexcept;
```

How long until it rings?

### restart

```cpp
void restart() noexcept;
```

### Start getter and setter

```cpp
const t_clock::time_point & getStart() const noexcept;
void setStart(t_clock::time_point start) noexcept;
```

### Duration getter and setter

```cpp
const seconds &getDuration() const noexcept;
void setDuration(const seconds & duration) noexcept;
```

### Helper types

```cpp
using t_duration = std::chrono::duration<double>;
using t_clock = std::chrono::system_clock;
using seconds = t_duration;
```
