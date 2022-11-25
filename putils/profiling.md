# [Profiling](profiling.hpp)

Profiling macros, currently wrapping [Tracy](https://github.com/wolfpld/tracy). To enable profiling, set the `PUTILS_PROFILING` CMake variable to `ON`.

## Members

### PUTILS_PROFILING_SCOPE

Placed at the start of a function, instruments it.

### PUTILS_PROFILING_FRAME

Placed in client code to mark the end of a frame.

## Cross-DLL usage

If multiple DLLs are going to link against `putils` with `PUTILS_PROFILING` enabled, you'll want to set the `TRACY_STATIC` CMake variable to `OFF`, to make sure all DLLs share a single instance of Tracy. You'll also want to use the [putils_copy_dlls](https://github.com/phisko/cmake_helpers/blob/main/CMakeModules/putils_copy_dlls.cmake) CMake function to make sure the Tracy DLL is copied next to your executable.