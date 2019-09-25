include(conan)

macro(putils_conan)
    conan_cmake_run(
        REQUIRES
            ${ARGV}
        BASIC_SETUP CMAKE_TARGETS
        BUILD missing)
endmacro()