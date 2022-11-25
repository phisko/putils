add_library(putils_lua INTERFACE)
target_include_directories(putils_lua INTERFACE .)

putils_conan_download_and_link_packages(
    putils_lua INTERFACE
    lua/5.4.4
    sol2/3.2.3
)