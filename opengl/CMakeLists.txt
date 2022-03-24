file(GLOB src *.cpp *.hpp *.inl)
add_library(putils_opengl STATIC ${src})

putils_conan(
    glew/2.2.0
    glm/0.9.9.8
)

target_link_libraries(putils_opengl putils CONAN_PKG::glew CONAN_PKG::glm)
target_compile_definitions(putils_opengl PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW PUTILS_GLM)