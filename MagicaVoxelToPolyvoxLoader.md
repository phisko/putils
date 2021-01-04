# [MagicaVoxelToPolyvoxLoader](MagicaVoxelToPolyvoxLoader.hpp)

## loadVoxFile

```cpp
static auto loadVoxFile(const char * f, putils::Point3i * outSize = nullptr) noexcept;
```

Loads a `.vox` file following the `MagicaVoxel` format into a `PolyVox::RawVolume`.