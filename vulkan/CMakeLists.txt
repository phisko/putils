file(GLOB src *.cpp *.hpp *.inl)
add_library(putils_vulkan STATIC ${src})

find_package(Vulkan REQUIRED)

target_link_libraries(putils_vulkan putils Vulkan::Vulkan)