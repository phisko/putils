# [get_call_stack](get_call_stack.hpp)

```cpp
std::string get_call_stack(int frames_to_ignore = 0) noexcept;
```

Returns a formatted call-stack, optionnally skipping a number of frames from the top of the stack.