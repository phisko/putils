file(GLOB src *.cpp *.hpp)
add_library(putils_mediator STATIC ${src})
target_link_libraries(putils_mediator putils)

if (UNIX)
    target_link_libraries(putils_mediator pthread)
endif()

if (PUTILS_TESTS)
    set(test_exe_name putils_mediator_tests)
    file(GLOB test_src tests/*.tests.cpp)

    add_executable(${test_exe_name} ${test_src})

    putils_conan(gtest/cci.20210126)
    target_link_libraries(${test_exe_name} CONAN_PKG::gtest putils_mediator)

    include(GoogleTest)
    gtest_discover_tests(${test_exe_name})
endif()