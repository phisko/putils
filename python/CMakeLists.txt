add_subdirectory(pybind11)

add_library(puthon INTERFACE)
target_link_libraries(puthon INTERFACE pybind11 module embed)
target_include_directories(puthon INTERFACE .)
