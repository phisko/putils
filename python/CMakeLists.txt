add_subdirectory(pybind11)

add_library(putils_python INTERFACE)
target_link_libraries(putils_python INTERFACE pybind11 module embed)
