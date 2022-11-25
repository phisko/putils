# [MagicaVoxelToPolyvoxLoader](MagicaVoxelToPolyvoxLoader.hpp)

## load_vox_file

```cpp
static auto load_vox_file(const char * f, putils::point3i * out_size = nullptr) noexcept;
```

Loads a `.vox` file following the `magica_voxel` format into a `PolyVox::RawVolume`.