putils_conan(
	pybind11/2.8.1
	GENERATORS cmake_find_package)

# Need to use cmake_find_package generator to use conan components (pybind11::embed and pybind11::module)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})
find_package(pybind11)

add_library(putils_python INTERFACE)
target_link_libraries(putils_python INTERFACE CONAN_PKG::pybind11 pybind11::embed pybind11::module)
