file(GLOB pseSrc *.cpp *.hpp)
add_library(putils_pse STATIC ${pseSrc})
target_include_directories(putils_pse PUBLIC .)

putils_conan(
    sfml/2.5.1@bincrafters/stable
    OPTIONS
        sfml:audio=True
        sfml:graphics=True
        sfml:window=True
)
target_link_libraries(putils_pse PUBLIC putils CONAN_PKG::sfml)