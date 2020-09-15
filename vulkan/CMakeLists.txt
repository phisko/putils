file(GLOB src *.cpp *.hpp)
add_library(putils_vulkan STATIC ${src})

conan_add_remote(NAME sesame URL https://api.bintray.com/conan/orhun/sesame)
putils_conan(vulkan/1.1.101.0@sesame/stable)

target_link_libraries(putils_vulkan CONAN_PKG::vulkan)
target_link_libraries(putils_vulkan putils)