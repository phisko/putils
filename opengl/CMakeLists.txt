file(GLOB src *.cpp *.hpp)
add_library(putils_opengl STATIC ${src})

set(BUILD_UTILS FALSE)
set(GLEW_PATH libs/glew)
add_subdirectory(${GLEW_PATH}/build/cmake)

target_include_directories(putils_opengl PUBLIC ${GLEW_PATH}/include)
target_link_libraries(putils_opengl putils glew)
target_compile_definitions(putils_opengl PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
