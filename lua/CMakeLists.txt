add_library(putils_lua INTERFACE)
target_include_directories(putils_lua INTERFACE .)

putils_conan(
    lua/5.4.4
    sol2/3.2.3
)
target_link_libraries(putils_lua INTERFACE CONAN_PKG::lua CONAN_PKG::sol2)
