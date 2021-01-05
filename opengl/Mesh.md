# [Mesh](Mesh.hpp)

Structure holding descriptors for an OpenGL mesh.

## Members

### vertexArrayObject

```cpp
putils::gl::VertexArray vertexArrayObject;
```

Ties the `vertexBuffer` and `indexBuffer` together.

### vertexBuffer

```cpp
putils::gl::Buffer vertexBuffer;
```

Holds the data for the vertices.

### indexBuffer

```cpp
putils::gl::Buffer indexBuffer;
```

Holds the data for the indices, used to avoid duplicating vertices.

### Indices metadata

```cpp
size_t nbIndices = 0;
GLenum indexType = GL_UNSIGNED_INT;
```

Index count and type, used for OpenGL operations.

## Helper function

### draw

```cpp
void draw(const Mesh & mesh) noexcept;
```

Binds `mesh`'s vertex array and buffer and calls `glDrawElements`.