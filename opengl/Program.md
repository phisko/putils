# [Program](Program.hpp)

Encapsulation of an OpenGL shader.

Automatically manages `uniform` locations and vertex attribute locations. Provides functions to expose a set of textures to debug.

Will report errors to `std::cerr` unless the `PUTILS_NO_SHADER_DEBUG` macro is defined.

### Members

##### Constructor

```cpp
Program(bool usesGBuffer = false, const char * name = "");
```

`usesGBuffer` specifies whether this shader will *read* from the GBuffer. `name` is only used for debug purposes.

##### use

```cpp
void use();
```

Must be called before performing any drawing operations to enable this particular shader.

##### drawObjects

```cpp
std::function<void(GLint modelLocation)> drawObjects;
```

Function provided externally to draw all objects.
`modelLocation` is the location of the model matrix `uniform` in this shader.

##### initWithShaders

```cpp
struct ShaderDescription {
    const char * src;
    GLenum type; // fragment, vertex, geometry
};

template<typename CRTP, size_t N>
void initWithShaders(const putils::vector<ShaderDescription, N> & shaders);
```

Uses the [reflectible API](../reflection/Reflectible.md) and gets all the attributes of `CRTP`, which are expected to match the `uniforms` declared in the shader sources. Sets the attribute values to the location of the `uniforms` so they may be used by the child class.

##### addGBufferTexture

```cpp
void addGBufferTexture(const char * name, int texture);
```

Sets the value for the *"g`name`"* `uniform` to `texture`.

### Non-member helpers

##### setVertexType

```cpp
template<typename T>
void setVertexType();
```

For each attribute in `T`'s reflectible API, enables the vertex attribute at that location and sets its offset accordingly.

##### setPolyVoxVertexType

```cpp
template<typename T>
void setPolyVoxVertexType();
```

Performs the same operations as `setVertexType` but for a `PolyVox` vertex type, which holds a *"position"*, *"normal"* and user-defined (reflectible) *"data"*.