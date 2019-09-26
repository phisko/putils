include(conan)

macro(putils_conan)
# usage: putils_conan(lib/1.0@man/stable)
    conan_check(REQUIRED)
    conan_add_remote(NAME bincrafters URL https://api.bintray.com/conan/bincrafters/public-conan)
    conan_add_remote(NAME conan-community URL https://api.bintray.com/conan/conan-community/conan)

    conan_cmake_run(
        REQUIRES
            ${ARGV}
        BASIC_SETUP CMAKE_TARGETS
        BUILD missing)
endmacro()
