# [RAII](RAII.hpp)

Scoped-limited, RAII-based representations of OpenGL abstractions.

```cpp
struct FrameBuffer;
struct Texture;
struct Buffer;
struct VertexArray;
```

## Members

### generate

```cpp
void generate() noexcept;
```

Allocates the underlying resource (for `FrameBuffer`, calls `glGenFramebuffers(1, &res)`).

### get

```cpp
GLuint & get() noexcept;
```

Returns the underlying resource.