# [Program](Program.hpp)

Encapsulation of an OpenGL shader. Requires the GLM library, and its presence to be specified by defining `PUTILS_GLM`.

Automatically manages `uniform` locations and vertex attribute locations. Provides functions to expose a set of textures to debug.

Will report errors to `std::cerr` unless the `PUTILS_NO_SHADER_DEBUG` macro is defined.

## Members

### Constructor

```cpp
Program(bool usesGBuffer = false, const char * name = "");
```

`usesGBuffer` specifies whether this shader will *read* from the GBuffer. `name` is only used for debug purposes.

### init

```cpp
virtual void init(size_t firstTextureID) noexcept = 0;
```

Called once the OpenGL context and GBuffer have been properly initialized.

### run

```cpp
struct Parameters {
    glm::mat4 view;
    glm::mat4 proj;
    glm::vec3 camPos;
    float camFOV;
    putils::Rect2i viewport;
    float nearPlane;
    float farPlane;
    size_t viewportID;
};
virtual void run(const Parameters & params) noexcept = 0;
```

Called each frame.

### use

```cpp
void use();
```

Must be called before performing any drawing operations to enable this particular shader.

### initWithShaders

```cpp
struct ShaderDescription {
    const char * src;
    GLenum type; // fragment, vertex, geometry
};

template<typename CRTP, size_t N>
void initWithShaders(const putils::vector<ShaderDescription, N> & shaders);
```

Uses the [reflectible API](../reflection.md) and gets all the attributes of `CRTP`, which are expected to match the `uniforms` declared in the shader sources. Sets the attribute values to the location of the `uniforms` so they may be used by the child class.

### addGBufferTexture

```cpp
void addGBufferTexture(const char * name, int texture);
```

Sets the value for the *"g`name`"* `uniform` to `texture`.

## Non-member helpers

### setVertexType

```cpp
template<typename T>
void setVertexType();
```

For each attribute in `T`'s reflectible API, enables the vertex attribute at that location and sets its offset accordingly.

### setPolyVoxVertexType

```cpp
template<typename T>
void setPolyVoxVertexType();
```

Performs the same operations as `setVertexType` but for a `PolyVox` vertex type, which holds a *"position"*, *"normal"* and user-defined (reflectible) *"data"*.