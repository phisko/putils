include(conan)

macro(putils_conan)
# usage: putils_conan(lib/1.0@man/stable)
    conan_check(REQUIRED)

    conan_cmake_run(
        REQUIRES
            ${ARGV}
        BASIC_SETUP CMAKE_TARGETS
        BUILD missing)
endmacro()