file(GLOB src *.cpp *.hpp)
add_library(putils_mediator STATIC ${src})
target_link_libraries(putils_mediator putils)

if (UNIX)
    target_link_libraries(putils_mediator pthread)
endif()

if (PUTILS_TESTS)
    set(test_exe_name putils_mediator_tests)
    file(GLOB test_src tests/*.tests.cpp)

    putils_add_test_executable(${test_exe_name} ${test_src})
    target_link_libraries(${test_exe_name} PRIVATE putils_mediator)
endif()