add_library(putils_python INTERFACE)

find_package(pybind11 CONFIG REQUIRED)
target_link_libraries(putils_python INTERFACE pybind11::embed)