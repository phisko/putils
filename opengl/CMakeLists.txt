file(GLOB src *.cpp *.hpp)
add_library(putils_opengl STATIC ${src})

set(BUILD_UTILS FALSE)
set(GLEW_PATH libs/glew)
add_subdirectory(${GLEW_PATH}/build/cmake)

putils_conan(
	glm/0.9.9.5@g-truc/stable
)

target_include_directories(putils_opengl PUBLIC ${GLEW_PATH}/include)
target_link_libraries(putils_opengl putils glew CONAN_PKG::glm)
target_compile_definitions(putils_opengl PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW PUTILS_GLM)