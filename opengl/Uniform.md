# [Uniform](Uniform.hpp)

```cpp
template<typename T>
struct Uniform;
```

Abstraction to an OpenGL uniform.

## Members

### location

```cpp
GLint location = -1;
```

Location of the uniform in the shader source.

### Assignment

```cpp
void operator=(const T & val) const noexcept;
```

Calls `glUniform*` for the corresponding type.