cmake_minimum_required(VERSION 3.0)
project(putils HOMEPAGE_URL "https://github.com/phisko/putils")

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNOMINMAX")
endif()

file(GLOB src *.cpp *.hpp reflection_helpers/*.cpp reflection_helpers/*.hpp)
add_library(putils STATIC ${src})
target_include_directories(putils PUBLIC .)

add_subdirectory(reflection)
target_link_libraries(putils PUBLIC reflection)

putils_conan(
    magic_enum/0.7.3
    nlohmann_json/3.10.5
    stb/cci.20210910
    termcolor/2.0.0
)
target_link_libraries(putils PUBLIC
    CONAN_PKG::magic_enum
    CONAN_PKG::nlohmann_json
    CONAN_PKG::stb
    CONAN_PKG::termcolor
)

option(PUTILS_PLUGIN_MANAGER "Build PluginManager" OFF)
if (PUTILS_PLUGIN_MANAGER)
    add_subdirectory(pluginManager)
    target_link_libraries(putils PUBLIC putils_pluginManager)
    target_compile_definitions(putils PUBLIC PUTILS_PLUGIN_MANAGER)
endif()

option(PUTILS_IMGUI "Build ImGui" OFF)
if (PUTILS_IMGUI)
    set(imguiSrc imgui/imgui.cpp imgui/imgui_widgets.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_demo.cpp)
    add_library(putils_imgui STATIC ${imguiSrc})
    target_include_directories(putils_imgui PUBLIC imgui)
    target_link_libraries(putils PUBLIC putils_imgui)
    target_compile_definitions(putils PUBLIC PUTILS_IMGUI)
endif()

option(PUTILS_LUA "Build Lua helper" OFF)
if(PUTILS_LUA)
    add_subdirectory(lua)
    target_link_libraries(putils PUBLIC putils_lua)
    target_compile_definitions(putils PUBLIC PUTILS_LUA)

    file(GLOB imgui_lua_src
        imgui_lua_bindings/*.cpp
        imgui_lua_bindings/*.hpp)
    add_library(putils_imgui_lua_bindings STATIC ${imgui_lua_src})
    target_link_libraries(putils_imgui_lua_bindings PUBLIC putils)
    target_link_libraries(putils PUBLIC putils_imgui_lua_bindings)
endif()

option(PUTILS_MEDIATOR "Build Mediator" OFF)
if(PUTILS_MEDIATOR)
    add_subdirectory(mediator)
    target_link_libraries(putils PUBLIC putils_mediator)
    target_compile_definitions(putils PUBLIC PUTILS_MEDIATOR)
endif()

option(PUTILS_PYTHON "Build Python helper" OFF)
if(PUTILS_PYTHON)
    add_subdirectory(python)
    target_link_libraries(putils PUBLIC putils_python)
    target_compile_definitions(putils PUBLIC PUTILS_PYTHON)
endif()

option(PUTILS_OPENGL "Build OpenGL helper" OFF)
if (PUTILS_OPENGL)
    add_subdirectory(opengl)
    target_link_libraries(putils PUBLIC putils_opengl)
    target_compile_definitions(putils PUBLIC PUTILS_OPENGL)
endif ()

option(PUTILS_VULKAN "Build Vulkan helper" OFF)
if (PUTILS_VULKAN)
    add_subdirectory(vulkan)
    target_link_libraries(putils PUBLIC putils_vulkan)
    target_compile_definitions(putils PUBLIC PUTILS_VULKAN)
endif()

option(PUTILS_NO_SHADER_DEBUG "Disable shader debug" OFF)
if (PUTILS_NO_SHADER_DEBUG)
    target_compile_definitions(putils PUBLIC PUTILS_NO_SHADER_DEBUG)
endif()

#
# Tests
#

option(PUTILS_TESTS "Build tests" OFF)
if (PUTILS_TESTS)
	enable_testing()
    set(test_exe_name putils_tests)
    file(GLOB test_src
            tests/*.tests.cpp
            reflection_helpers/tests/*.tests.cpp)

    add_executable(${test_exe_name} ${test_src})

    putils_conan(gtest/cci.20210126)
    target_link_libraries(${test_exe_name} CONAN_PKG::gtest putils)

    include(GoogleTest)
    gtest_discover_tests(${test_exe_name})
endif()
