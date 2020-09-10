file(GLOB src *.cpp *.hpp)
add_library(putils_mediator STATIC ${src})
target_include_directories(putils_mediator PUBLIC .)
target_link_libraries(putils_mediator putils)

if (UNIX)
    target_link_libraries(putils_mediator pthread)
endif()
