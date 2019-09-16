# Reflection

A simple, stand-alone, header-only and easily pluggable reflection system.

This does not allow you to introspect any type provided by any library. It simply provides an API that you can implement in your own classes so that you can introspect them.
 
A good example of this is the [Serializable](Serializable.md) class, that uses reflection to inspect itself and serialize its attributes.

Another example is the [registerType](../lua/README.md) function provided for lua, that inspects a type and registers all its attributes and methods to a lua state.

### Classes

* [Reflectible](Reflectible.md): enforces the reflection API proposed by this library, and provides a helper is_reflectible type-trait.
* [Serializable](Serializable.md) Mix-in class letting any derived class serialize its attributes with great simplicity.
